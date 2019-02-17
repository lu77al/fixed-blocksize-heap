#include "heap.h"
#include <cstring>

Heap::Heap(void* memory, size_t memorySize, size_t blockSize)
{
	this->blockSize = blockSize;
	status = (char *)memory;
	freeSpace = capacity = memorySize / (blockSize + 1);
	if (capacity == 0) return;
	buffer = status + capacity;
	memset(status, 0, capacity);
}

void* Heap::allocate()
{
	return nullptr;
}

void Heap::free(void* p)
{
}
