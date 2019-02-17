#include "heap.h"
#include <cstring>

Heap::Heap(void* memory, size_t memorySize, size_t blockSize)
{
	this->blockSize = blockSize;
	status = (char*)memory;
	freeSpace = capacity = memorySize / (blockSize + 1);
	if (capacity == 0) return;
	buffer = status + capacity;
	memset(status, 0, capacity);
}

void* Heap::allocate()
{
	if (freeSpace == 0) return nullptr;
	char* pos = (char*)memchr(status, 0, capacity);
	if (pos == nullptr) return nullptr;
	freeSpace--;
	*pos = 1;
	return buffer + (pos - status) * blockSize;
}

void Heap::free(void* p)
{
}
