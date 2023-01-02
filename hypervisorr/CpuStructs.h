#pragma once
#include <ntifs.h>
#include "cpu_codes.h"
#include "vmcb.h"



typedef struct Virtual_Processor_Data
{
	VMCB host_VMCB;
	VMCB guest_VMCB;
	
	DECLSPEC_ALIGN(PAGE_SIZE) BYTE host_StateArea[PAGE_SIZE];

	PVMCB ptr_guest_VMCB;				// PHYSICAL ADDRESS
	PVMCB ptr_host_VMCB;				// PHYSICAL ADDRESS
	PVirtual_Processor_Data ptr_Self;
	PT_4L_long2mb::PPAGING_DATA paging_Data;
	
}Virtual_Processor_Data, *PVirtual_Processor_Data;



typedef struct Register_Context
{
	uintptr_t r15;
	uintptr_t r14;
	uintptr_t r13;
	uintptr_t r12;
	uintptr_t r11;
	uintptr_t r10;
	uintptr_t r9;
	uintptr_t r8;
	uintptr_t rdi;
	uintptr_t rsi;
	uintptr_t rbp;
	//uintptr_t rsp;
	uintptr_t rdx;
	uintptr_t rcx;
	uintptr_t rbx;
	uintptr_t rax;
}Register_Context, *PRegister_Context;





namespace segment_long {
	typedef struct _SEGMENT_REGISTER
	{
		WORD Limit;
		uintptr_t Base;
	}SEGMENT_REGISTER, PSEGMENT_REGISTER;


	typedef struct _SEGMENT_DESCRIPTOR
	{
		WORD SegLimitLow;
		WORD BaseAddress;
		BYTE BaseLow;
		uintptr_t Type :			4;
		uintptr_t System :			1;
		uintptr_t Dpl :				2;
		uintptr_t Present :			1;
		uintptr_t SegLimitHigh :	4;
		uintptr_t Avl :				1;
		uintptr_t LongMode :		1;
		uintptr_t Db :				1;
		uintptr_t Granularity :		1;
		uintptr_t BaseHigh :		8;
	}SEGMENT_DESCRIPTOR, *PSEGMENT_DESCRIPTOR;

	typedef struct _SEGMENT_ATTRIBUTE
	{
		UINT16 Type :				4;
		UINT16 System :				1;
		UINT16 Dpl :				2;
		UINT16 Present :			1;
		UINT16 Avl :				1;
		UINT16 LongMode :			1;
		UINT16 DefaultBit :			1;
		UINT16 Granularity :		1;
		UINT16 Reserved1 :			4;
	}SEGMENT_ATTRIBUTE, * PSEGMENT_ATTRIBUTE;

}

namespace PT_4L_long4kb
{
	typedef struct _PML4_ENTRY_4kb
	{
		uintptr_t Present :			1;
		uintptr_t ReadWrite :		1;
		uintptr_t User :			1;
		uintptr_t WriteThrough :	1;
		uintptr_t Pcd :				1;
		uintptr_t Accessed :		1;
		uintptr_t Reserved :		3;
		uintptr_t Avl :				3;
		uintptr_t PdpBasePhysical : 40;
		uintptr_t Reserved2 :		11;
		uintptr_t NoExecute :		1;
	}PML4_ENTRY_4kb, * PPML4_ENTRY_4kb;
	static_assert(sizeof(PML4_ENTRY_4kb) == 8, "PML4_ENTRY_4kb Size Mismatch");


	typedef struct _PDP_ENTRY_4kb
	{
		uintptr_t Present :			1;
		uintptr_t ReadWrite :		1;
		uintptr_t User :			1;
		uintptr_t WriteThrough :	1;
		uintptr_t Pcd :				1;
		uintptr_t Accessed :		1;
		uintptr_t Reserved :		3;
		uintptr_t Avl :				3;
		uintptr_t PdBasePhysical :	40;
		uintptr_t Reserved2 :		11;
		uintptr_t NoExecute :		1;
	}PDP_ENTRY_4kb, *PPDP_ENTYR_4kb;
	static_assert(sizeof(PDP_ENTRY_4kb) == 8, "PDP_ENTRY_4kb Size Mismatch");

	typedef struct _PD_ENTRY_4kb
	{
		uintptr_t Present :			1;
		uintptr_t ReadWrite :		1;
		uintptr_t User :			1;
		uintptr_t WriteThrough :	1;
		uintptr_t Pcd :				1;
		uintptr_t Accessed :		1;
		uintptr_t Reserved :		3;
		uintptr_t Avl :				3;
		uintptr_t PdBasePhysical :	40;
		uintptr_t Reserved2 :		11;
		uintptr_t NoExecute :		1;
	}PD_ENTRY_4kb, *PPD_ENTRY_4kb;
	static_assert(sizeof(PD_ENTRY_4kb) == 8, "PD_ENTRY_4kb Size Mismatch");


	typedef struct _PT_ENTRY_4kb
	{
		uintptr_t Present : 1;
		uintptr_t ReadWrite : 1;
		uintptr_t User : 1;
		uintptr_t WriteThrough : 1;
		uintptr_t Pcd : 1;
		uintptr_t Accessed : 1;
		uintptr_t Dirty : 1;
		uintptr_t PAT : 1;
		uintptr_t Global : 1;
		uintptr_t Avl : 3;
		uintptr_t PageBasePhysical : 40;
		uintptr_t Reserverd : 7;
		uintptr_t CR4_PKE : 4;
		uintptr_t NoExecute : 1;
	}PT_ENTRY_4kb, *PPT_ENTRY_4kb;
	static_assert(sizeof(PT_ENTRY_4kb) == 8, "PT_ENTRY_4kb Size Mismatch");

	typedef struct PAGING_DATA
	{
		PVOID MsrPermBitMap;
		DECLSPEC_ALIGN(PAGE_SIZE) PML4_ENTRY_4kb	Pml4Entry[1];
		DECLSPEC_ALIGN(PAGE_SIZE) PDP_ENTRY_4kb		PdpEntries[512];
		DECLSPEC_ALIGN(PAGE_SIZE) PD_ENTRY_4kb		PdeEntries[512];
		DECLSPEC_ALIGN(PAGE_SIZE) PT_ENTRY_4kb		PtEntries[512];

	} PAGING_DATA, * PPAGING_DATA;


};



namespace PT_4L_long2mb
{
	typedef struct _PML4_ENTRY_2MB
	{
		uintptr_t Valid : 1;
		uintptr_t Write : 1;
		uintptr_t User : 1;
		uintptr_t WriteThrough : 1;
		uintptr_t CacheDisable : 1;
		uintptr_t Accessed : 1;
		uintptr_t Reserved : 3;
		uintptr_t Avl : 3;
		uintptr_t PageFrameNumber : 40;
		uintptr_t Reserved2 : 11;
		uintptr_t NoExecute : 1;
	}PML4_ENTRY_2MB, * PPML4_ENTRY_2MB, PDP_ENTRY_2MB, * PPDP_ENTRY_2MB;
	static_assert(sizeof(PML4_ENTRY_2MB) == 8, "PML4_ENTRY_2MB Size Mismatch");


	typedef struct _PD_Entry_2MB
	{
		uintptr_t Valid : 1;
		uintptr_t Write : 1;
		uintptr_t User : 1;
		uintptr_t WriteThrough : 1;
		uintptr_t CacheDisable : 1;
		uintptr_t Accessed : 1;
		uintptr_t Dirty : 1;
		uintptr_t PS : 1;
		uintptr_t Global : 1;
		uintptr_t Avl : 3;
		uintptr_t Pat : 1;
		uintptr_t Reserved : 8;
		uintptr_t PageFrameNumber : 31;
		uintptr_t Reserved2 : 11;
		uintptr_t NoExecute : 1;
	}PD_ENTRY_2MB, * PPD_ENTRY_2MB;
	static_assert(sizeof(PD_ENTRY_2MB) == 8, "PDE_ENTRY_2MB Size Mismatch");


	typedef struct _PAGING_DATA
	{
		PVOID MsrPermBitMap;
		DECLSPEC_ALIGN(PAGE_SIZE) PML4_ENTRY_2MB PML4[1];					// Page Map Level 4
		DECLSPEC_ALIGN(PAGE_SIZE) PDP_ENTRY_2MB PDPT[512];					// Page Directory Pointer Table
		DECLSPEC_ALIGN(PAGE_SIZE) PD_ENTRY_2MB PD[512][512];				// Page Directory
	}PAGING_DATA, *PPAGING_DATA;
	

}