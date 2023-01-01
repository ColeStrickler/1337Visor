#include "InitVirt.h"



void virt::BuildPT4L_4kb(_Out_ PT_4L_long4kb::PPAGING_DATA PagingData)
{

	uintptr_t PdpBasePa, PdBasePa, PtBasePa, translationPa;

	PdpBasePa = MmGetPhysicalAddress(&PagingData->PdpEntries).QuadPart;
	PagingData->Pml4Entry[0].PdpBasePhysical = PdpBasePa >> PAGE_SHIFT;

	PagingData->Pml4Entry[0].Present = 1;
	PagingData->Pml4Entry[0].ReadWrite = 1;	// allow r/w
	PagingData->Pml4Entry[0].User = 1;  // all guest accesses treated as user at the nested level

	for (short int i = 0; i < 512; i++)
	{
		PdBasePa = MmGetPhysicalAddress(&PagingData->PdeEntries[i]).QuadPart;
		PagingData->PdpEntries[i].PdBasePhysical = PdBasePa >> PAGE_SHIFT;

	}



};


void virt::BuildPT4L_2MB(_Out_ PT_4L_long2mb::PPAGING_DATA PagingData)
{
	uintptr_t PdpPhysicalBase, PdePhysicalBase, TranslationPhysical;

	// 1 PML4E controls up to 512Gb phyiscal memory
	// Using larger page sizes decreases number of TLB misses and therefore results
	// in better virtualization performance
	// 
	// 
	// We set the page frame number to the physical base of the Page Directory Pointer table
	// We must shift the entry by 12 bits, because the 40 bits of the PFN are treated as the 
	// most significant bits of the 52bit physical address and bits 0-11 are assumed zero
	// since it is page aligned
	PdpPhysicalBase = MmGetPhysicalAddress(&PagingData->PDPT).QuadPart;
	PagingData->PML4[0].PageFrameNumber = PdpPhysicalBase >> 12;

	// Treat all guest accesses as user
	PagingData->PML4[0].User = 1;
	// All guest pages are treated as write access
	PagingData->PML4[0].Write = 1;
	// Must mark valid or otherwise lower hierarchical page entries are treated invalid
	PagingData->PML4[0].Valid = 1;


	// See page 150 AMD manual figure 5-25 for a graphical overview of how 4 level 2Mb paging works
	for (short int i = 0; i < 512; i++)
	{
		// One Page Directory controls 512 pages
		PdePhysicalBase = MmGetPhysicalAddress(&PagingData->PD[i][0]).QuadPart;
		PagingData->PDPT[i].PageFrameNumber = PdePhysicalBase >> 12;
		PagingData->PDPT[i].Valid = 1;
		PagingData->PDPT[i].Write = 1;
		PagingData->PDPT[i].User = 1;

		// We could insert some PAT/MTRR caching optimizations at this point if warranted
		// via the PAT, PCD, PWT bits in the page tables but I need to do further research
		// into this so i can do a correct implementation

		for (short int j = 0; j < 512; j++) {


			// We do this because it is an index into the actual page tables of the host
			PagingData->PD[i][j].PageFrameNumber = (i * 512) + j;
			PagingData->PD[i][j].Valid = 1;
			PagingData->PD[i][j].Write = 1;
			PagingData->PD[i][j].User = 1;
			PagingData->PD[i][j].PS = 1; // activate 2Mb pages here
		}
	}
}



void virt::SetMsrPermissionsMap(PVOID MsrPermissionsMapMemory)
{
	// See AMD Manual 15.11 "MSR Intercepts"
	// MSRPM BYTE OFFSET : MSR RANGE
	//		000h-7FFh		0000_0000h - 0000_1FFFh
	//		800h-FFFh		C000_0000h - C000_1FFFh
	//		1000h-17FFh		C001_0000h - C001_1FFFh
	//		1800h-1FFFh		Reserved

	RTL_BITMAP MSR_PERMISSION_BITMAP;
	constexpr DWORD BITS_PER_MSR = 2;
	constexpr WORD VECTOR_SIZE = 0X4000;

	// We can use this base to calculate where the MSR is represented in the bitmap
	// We do this like so
	// offset = ((msr - base) * bits/msr) + (vector_size * i)
	// where i is an index into the MSR range
	// We will use i = 1, because we are picking MSRs from C000_0000h - C000_1FFFh
	constexpr DWORD MSR_RANGE0_BASE = 0x00000000;
	constexpr DWORD MSR_RANGE1_BASE = 0xC0000000;
	constexpr DWORD MSR_RANGE2_BASE = 0xC0011FFF;


	RtlInitializeBitMap(&MSR_PERMISSION_BITMAP, (PULONG)MsrPermissionsMapMemory, VECTOR_SIZE * 4);
	RtlClearAllBits(&MSR_PERMISSION_BITMAP);


	// SELECT BIT OFFSETS FOR MSRs WE WANT TO INTERCEPT
	ULONG offsetEFER_bits = ((IA32_EFER - MSR_RANGE1_BASE)) + VECTOR_SIZE;




	// Now we can set bit @offset for intercepting read access
	// and we can set bit @(offset + 1) for intercepting write access
	RtlSetBits(&MSR_PERMISSION_BITMAP, offsetEFER_bits, 1);
	RtlSetBits(&MSR_PERMISSION_BITMAP, offsetEFER_bits + 1, 1);



}



NTSTATUS virt::VirtualizeProcessors(Globals* g_GlobalSettings, _In_opt_ PVOID PagingData, _Out_ LPDWORD NumVirtualized, PagingModes ActivePagingMode)
{
	NTSTATUS ret = STATUS_SUCCESS;

	ULONG NumberOfLogicalProcessors;
	PROCESSOR_NUMBER ProcNum;
	GROUP_AFFINITY NewAffinity, OldAffinity;

	NumberOfLogicalProcessors = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);

	for (short int i = 0; i < NumberOfLogicalProcessors; i++)
	{
		ret = KeGetProcessorNumberFromIndex((ULONG)i, &ProcNum);
		if (!NT_SUCCESS(ret)) {
			*NumVirtualized = i;
			return ret;
		}

		// SWITCH CODE EXECUTION TO THE SELECTED PROCESSOR
		NewAffinity.Group = ProcNum.Group;
		NewAffinity.Mask = 1ULL << ProcNum.Number;
		NewAffinity.Reserved[0] = NewAffinity.Reserved[1] = NewAffinity.Reserved[2] = 0;
		KeSetSystemGroupAffinityThread(&NewAffinity, &OldAffinity);
		
		ret = VirtualizeSingleProcessor(g_GlobalSettings, PagingData);


		// Switch back to old processor affinity
		KeRevertToUserGroupAffinityThread(&OldAffinity);
		if (!NT_SUCCESS(ret)) {
			*NumVirtualized = i;
			return ret;
		}

	}

}


WORD virt::GetSegmentAttribute(WORD SegmentSelector, uintptr_t ptr_GDT)
{
	segment_long::PSEGMENT_DESCRIPTOR Descriptor;
	segment_long::SEGMENT_ATTRIBUTE Attribute16;			// sizeof(segment_long::SEGMENT_ATTRIBUTE) = 0x10


	// Index into the Global Descriptor table to select the corresponding segment descriptor
	Descriptor = (segment_long::PSEGMENT_DESCRIPTOR)(ptr_GDT + (SegmentSelector & ~0x3)); // ignore requestor privilege level


	Attribute16.Type = Descriptor->Type;
	Attribute16.System = Descriptor->System;
	Attribute16.Dpl = Descriptor->Dpl;
	Attribute16.Present = Descriptor->Present;
	Attribute16.Avl = Descriptor->Avl;
	Attribute16.LongMode = Descriptor->LongMode;
	Attribute16.DefaultBit = Descriptor->Db;
	Attribute16.Granularity = Descriptor->Granularity;
	Attribute16.Reserved1 = 0;


	WORD AttributeUINT16 = 0;
	memcpy(&AttributeUINT16, &Attribute16, sizeof(WORD));
	
	return AttributeUINT16;
}


void virt::BuildVMCB(Globals* g_GlobalSettings, PVirtual_Processor_Data PtrVprocData, PVOID PagingData, PCONTEXT InitializationContext)
{
	segment_long::SEGMENT_REGISTER IDTR, GDTR;
	LARGE_INTEGER GuestVMCB, HostVMCB, HostStateArea, PML4, MSRPM;

	// For some reason _sgdt was not in the wdk header file so we manually put it back in
	// It already exists in the microsoft header file, but visual studio doesnt wanna look in that one
	// CHECK HERE FIRST FOR ERRORS
	_sgdt(&GDTR);
	__sidt(&IDTR);

	PT_4L_long2mb::PPAGING_DATA page_data = (PT_4L_long2mb::PPAGING_DATA)PagingData;



	GuestVMCB = MmGetPhysicalAddress(&PtrVprocData->guest_VMCB);
	HostVMCB = MmGetPhysicalAddress(&PtrVprocData->host_VMCB);
	HostStateArea = MmGetPhysicalAddress(&PtrVprocData->host_StateArea);
	PML4 = MmGetPhysicalAddress(&page_data->PML4);			
	MSRPM = MmGetPhysicalAddress(page_data->MsrPermBitMap);


	// CONFIGURE OUR INTERCEPTS HERE
	PtrVprocData->guest_VMCB.ControlArea.InterceptMisc1.IntrCPUID = 1;			// We modify the vendor string to a custom value
	PtrVprocData->guest_VMCB.ControlArea.InterceptMisc2.IntrVMRUN = 1;			// This is required. See page 505 AMD64 Architecture Programmer’s Manual, Volume 2: System Programming
	PtrVprocData->guest_VMCB.ControlArea.InterceptMisc1.IntrMSR = 1;			// Activate MSRPM bitmap intercepts
	

	// Since this is not going to be a multi guest hypervisor we can just set all ASID to 1
	PtrVprocData->guest_VMCB.ControlArea.GuestAsid = 1;


	// ENABLE NESTED PAGING
	// We may do some editing here in the future to enable more of the available settings
	PtrVprocData->guest_VMCB.ControlArea.NpEnable.NP_ENABLE = 1;


	// SET UP PHYSICAL ADDRESSES
	PtrVprocData->guest_VMCB.ControlArea.MSRPM_BASE_PA = MSRPM.QuadPart;
	PtrVprocData->guest_VMCB.ControlArea.NCr3 = PML4.QuadPart;			// Set guest CR3 to Page Map Level 4 of our created Nested Page Tables

	

	// SETUP GUEST INITIALIZATION STATE
	PtrVprocData->guest_VMCB.StateSaveArea.gdtr.base_addr = GDTR.Base;
	PtrVprocData->guest_VMCB.StateSaveArea.gdtr.limit = GDTR.Limit;
	PtrVprocData->guest_VMCB.StateSaveArea.idtr.base_addr = IDTR.Base;
	PtrVprocData->guest_VMCB.StateSaveArea.idtr.limit = IDTR.Limit;
	//CS
	PtrVprocData->guest_VMCB.StateSaveArea.cs.selector = InitializationContext->SegCs;
	PtrVprocData->guest_VMCB.StateSaveArea.cs.limit = GetSegmentLimit(InitializationContext->SegCs);
	PtrVprocData->guest_VMCB.StateSaveArea.cs.attribute = GetSegmentAttribute(InitializationContext->SegCs, GDTR.Base);
	//DS
	PtrVprocData->guest_VMCB.StateSaveArea.ds.selector = InitializationContext->SegDs;
	PtrVprocData->guest_VMCB.StateSaveArea.ds.limit = GetSegmentLimit(InitializationContext->SegDs);
	PtrVprocData->guest_VMCB.StateSaveArea.ds.attribute = GetSegmentAttribute(InitializationContext->SegDs, GDTR.Base);
	//ES
	PtrVprocData->guest_VMCB.StateSaveArea.es.selector = InitializationContext->SegEs;
	PtrVprocData->guest_VMCB.StateSaveArea.es.limit = GetSegmentLimit(InitializationContext->SegEs);
	PtrVprocData->guest_VMCB.StateSaveArea.es.attribute = GetSegmentAttribute(InitializationContext->SegEs, GDTR.Base);
	//SS
	PtrVprocData->guest_VMCB.StateSaveArea.ss.selector = InitializationContext->SegSs;
	PtrVprocData->guest_VMCB.StateSaveArea.ss.limit = GetSegmentLimit(InitializationContext->SegSs);
	PtrVprocData->guest_VMCB.StateSaveArea.ss.attribute = GetSegmentAttribute(InitializationContext->SegSs, GDTR.Base);

	PtrVprocData->guest_VMCB.StateSaveArea.Efer = __readmsr(IA32_EFER);
	PtrVprocData->guest_VMCB.StateSaveArea.Cr0 = __readcr0();
	PtrVprocData->guest_VMCB.StateSaveArea.Cr2 = __readcr2();
	PtrVprocData->guest_VMCB.StateSaveArea.Cr3 = __readcr3();
	PtrVprocData->guest_VMCB.StateSaveArea.Cr4 = __readcr4();

	PtrVprocData->guest_VMCB.StateSaveArea.Rflags = InitializationContext->EFlags;
	PtrVprocData->guest_VMCB.StateSaveArea.Rsp = InitializationContext->Rsp;
	PtrVprocData->guest_VMCB.StateSaveArea.Rip = InitializationContext->Rip;
	PtrVprocData->guest_VMCB.StateSaveArea.GPat = __readmsr(MSR_PAT);


	
	// __svm_vmsave is the functional equivalent of the VMSAVE instruction --> https://learn.microsoft.com/en-us/cpp/intrinsics/svm-vmsave?view=msvc-170
	// VMSAVE saves more state that isnt readily accessible to the VMCB
	// Check out the AMD Manual for gritty details of VMSAVE
	__svm_vmsave(GuestVMCB.QuadPart);
	
	// Save data to stack for use in assembly functions
	PtrVprocData->ptr_Self = PtrVprocData;
	PtrVprocData->paging_Data = page_data;
	PtrVprocData->ptr_host_VMCB = (PVMCB)(HostVMCB.QuadPart);
	PtrVprocData->ptr_guest_VMCB = (PVMCB)(GuestVMCB.QuadPart);
	

	// We set MSR_VM_HSAVE_PA to our PAGE_SIZE Byte array
	// VMRUN and will save some processor state here, and #VMEXIT will load that state back
	__writemsr(MSR_VM_HSAVE_PA, HostStateArea.QuadPart);


	__svm_vmsave(HostVMCB.QuadPart);

}


NTSTATUS virt::VirtualizeSingleProcessor(Globals* g_GlobalSettings, _In_ PVOID PagingData)
{
	NTSTATUS ret = STATUS_SUCCESS;
	PVirtual_Processor_Data P_vproc_data = nullptr;
	PCONTEXT ContextInit = (PCONTEXT)ExAllocatePoolWithTag(NonPagedPool, sizeof(CONTEXT), 'hypr');

	if (ContextInit == nullptr) {
		KdPrint(("[virt::VirtualizeSingleProcessor]: Unable to allocate memory."));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	P_vproc_data = (PVirtual_Processor_Data)kernelmem::AllocPageAlignedPhysicalMem(sizeof(Virtual_Processor_Data));
	RtlCaptureContext(ContextInit);

	if (isInstalled())
	{


		// enable SVM
		__writemsr(IA32_EFER, __readmsr(IA32_EFER) | IA32_EFER_SVME);

		BuildVMCB(g_GlobalSettings, P_vproc_data, PagingData, ContextInit);


		EnterSVM()

	}

}



NTSTATUS virt::InitVirtualization(Globals* g_GlobalSettings)
{
	NTSTATUS status = STATUS_SUCCESS;
	PT_4L_long2mb::PPAGING_DATA PagingData = nullptr;



	PagingData = (PT_4L_long2mb::PPAGING_DATA)kernelmem::AllocPageAlignedPhysicalMem(sizeof(PT_4L_long2mb::PAGING_DATA));
	if (!PagingData) {
		KdPrint(("Could not allocate memory for PAGING_DATA\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	g_GlobalSettings->PageData = PagingData;


	PagingData->MsrPermBitMap = kernelmem::AllocPageAlignedPhysicalMem(MSRPERM_MAP_SIZE);
	if (!PagingData->MsrPermBitMap) {
		KdPrint(("Could not allocate memory for PAGING_DATA->MsrPermBitMap\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	BuildPT4L_2MB(PagingData);
	SetMsrPermissionsMap(PagingData->MsrPermBitMap);


	DWORD processorsVirtualized = 0;
	status = VirtualizeProcessors(g_GlobalSettings, PagingData, &processorsVirtualized, virt::PagingModes::TwoMb);

	if (!NT_SUCCESS(status)) {

		if (processorsVirtualized > 0) {
			ExitVirtualSession();
		}


	}


}



