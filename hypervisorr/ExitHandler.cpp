#include "ExitHandler.h"



bool exit_handler::exitcheck(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx)
{

	KIRQL currIRQL;
	bool exit = FALSE;

	__svm_vmload((uintptr_t)VprocData->ptr_host_VMCB);

	if (currIRQL < DISPATCH_LEVEL)
	{
		KeRaiseIrqlToDpcLevel();
	}


	RegCtx->rax = VprocData->guest_VMCB.StateSaveArea.Rax;

	switch (VprocData->guest_VMCB.ControlArea.ExitCode)
	{

		case VMEXIT::_CPUID:
		{
			exit = exit_handler::cpuid_handler(VprocData, RegCtx);
		}

		case VMEXIT::_MSR:
		{
			exit = exit_handler::msr_access_handler(VprocData, RegCtx);
		}

		case VMEXIT::_VMRUN:
		{
			exit = exit_handler::vmrun_handler(VprocData, RegCtx);
		}

		default:
			KeBugCheck(MANUALLY_INITIATED_CRASH);

	}

	if (currIRQL < DISPATCH_LEVEL)
	{
		KeLowerIrql(currIRQL);
	}


	if (exit)
	{
		RegCtx->rax = reinterpret_cast<uintptr_t>(VprocData) & MAXUINT32;			// eax:edx contains per processor data to be freed
		RegCtx->rbx = VprocData->guest_VMCB.ControlArea.NRip;						// the next instruction pointer to be used after return
		RegCtx->rcx = VprocData->guest_VMCB.StateSaveArea.Rsp;						// restore the stack pointer
		RegCtx->rdx = reinterpret_cast<uintptr_t>(VprocData) >> 32;					// eax:edx contains per processor data to be freed

		// load guest state
		__svm_vmload((uintptr_t)VprocData->ptr_guest_VMCB);


		// disable interrupts
		_disable();
		// set Global Interrupt Flag(GIF) as required to exit virtualization
		__svm_stgi();	// these compiler intrinsics by microsoft are dope, shout out the gang

		
		__writemsr(IA32_EFER, __readmsr(IA32_EFER) & ~IA32_EFER_SVME);
		__writeeflags(VprocData->guest_VMCB.StateSaveArea.Rflags);

		return exit;
	}

	// Push potentially updated RAX into VMCB
	// This is loaded back during VMRUN
	VprocData->guest_VMCB.StateSaveArea.Rax = RegCtx->rax;
	return exit;
}


bool exit_handler::cpuid_handler(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx)
{
	int cpuinfo[4] {0, 0, 0, 0};
	bool exit = FALSE;

	segment_long::_SEGMENT_ATTRIBUTE attrib;
	memcpy(&attrib, &VprocData->guest_VMCB.StateSaveArea.ss.attribute, sizeof(attrib));

	int function = RegCtx->rax;
	int subleaf = RegCtx->rcx;

	__cpuidex(cpuinfo, function, subleaf);		// actually execute the function


	switch (unsigned int(function))
	{

		case CPUID_PROCESSOR_FEATURE_IDENTIFIERS:
		{
			cpuinfo[2] |= CPUID_FN0000_0001_ECX_HV_PRESENT;
			break;
		}

		case CPUID_MICROSOFT_HV_INTERFACE:
		{
			cpuinfo[0] = '0#vH';
			break;
		}

		case CPUID_HYPERVISOR_VENDOR_ID:
		{
			cpuinfo[0] = 0x40000001;
			cpuinfo[1] = '7331';
			cpuinfo[2] = 'osiv';
			cpuinfo[3] = '   r';
			break;
		}

		case CPUID_EXIT_VIRTUAL_SESSION:
		{
			if (attrib.Dpl == 0) {
				exit = true;
			}
			break;
		}
		default:
			break;
	}

	RegCtx->rax = cpuinfo[0];
	RegCtx->rbx = cpuinfo[1];
	RegCtx->rcx = cpuinfo[2];
	RegCtx->rdx = cpuinfo[3];

	VprocData->guest_VMCB.StateSaveArea.Rip = VprocData->guest_VMCB.ControlArea.NRip;
}


bool exit_handler::msr_access_handler(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx)
{

}

bool exit_handler::vmrun_handler(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx)
{

}