#pragma once
#include <ntifs.h>


namespace kernelmem
{
	PVOID AllocPageAlignedPhysicalMem(_In_ size_t NumBytes);
	PVOID AllocContiguousPhysicalMem(_In_ size_t NumBytes);

};
