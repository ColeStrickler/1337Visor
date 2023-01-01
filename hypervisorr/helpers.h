#pragma once
#include <ntifs.h>
#include "cpu_codes.h"
#include "InitVirt.h"


#define uint32 unsigned int
#define BOOL bool



struct Globals {
	PVOID						PowerStateCallback;

	virt::PagingModes			ActivePageMode;
	PVOID						PageData;



};







extern "C" uint32 call_cpuid(uint32 eax_in_val, uint32 ecx_in_val, uint32 * eax_out_ptr, uint32 * ebx_out_ptr, uint32 * ecx_out_ptr, uint32 * edx_out_ptr);

