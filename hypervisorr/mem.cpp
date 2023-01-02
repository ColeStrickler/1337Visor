#include "mem.h"



PVOID kernelmem::AllocPageAlignedPhysicalMem(_In_ size_t NumBytes)
{
	// This will not work if the number of bytes is less than a Page's worth
	NT_ASSERT(NumBytes >= PAGE_SIZE);

	PVOID allocMem = nullptr;
	allocMem = ExAllocatePoolWithTag(NonPagedPool, NumBytes, 'hypr');

	if (allocMem == nullptr) {
		return allocMem;
	}

	NT_ASSERT(PAGE_ALIGN(allocMem) == allocMem);
	memset(allocMem, 0x00, NumBytes);
	return allocMem;
}



PVOID kernelmem::AllocContiguousPhysicalMem(_In_ size_t NumBytes)			// Must be freed with MmFreeContiguousMemory
{

	LARGE_INTEGER bounds, low, high;
	bounds.QuadPart = low.QuadPart = 0;
	high.QuadPart = -1;

	PVOID allocMem = MmAllocateContiguousMemorySpecifyCacheNode(NumBytes, low, high, bounds, MmCached, MM_ANY_NODE_OK);
	if (!allocMem) {
		return nullptr;
	}
	memset(allocMem, 0x00, NumBytes);
	return allocMem;

}