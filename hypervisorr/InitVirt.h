#pragma once
#include <ntifs.h>
#include "CpuStructs.h"
#include "helpers.h"
#include "mem.h"
#include <intrin.h>
namespace virt
{


	enum PagingModes : short
	{
		FourKb,
		TwoMb,
		FourMb,
		OneGb

	};


	NTSTATUS InitVirtualization(Globals* g_GlobalSettings);
	NTSTATUS VirtualizeProcessors(Globals* g_GlobalSettings, _In_ PVOID PagingData, _Out_ LPDWORD NumVirtualized, PagingModes ActivePagingMode);
	NTSTATUS VirtualizeSingleProcessor(Globals* g_GlobalSettings, _In_ PVOID PagingData);
	void BuildPT4L_2MB(_Out_ PT_4L_long2mb::PPAGING_DATA PagingData);
	void BuildPT4L_4kb(_Out_ PT_4L_long4kb::PPAGING_DATA PagingData);
	void SetMsrPermissionsMap(PVOID MsrPermissionsMapMemory);
	void ExitVirtualSession();
	WORD GetSegmentAttribute(WORD SegmentSelector, uintptr_t ptr_GDT);
	void BuildVMCB(Globals* g_GlobalSettings, PVirtual_Processor_Data PtrVprocData, PVOID PagingData, PCONTEXT InitializationContext);
	bool isInstalled();

}

