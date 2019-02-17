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
	void* result = tryAllocate();
	if (result == nullptr) throw "Not enough space to allocate block";
	return result;
}

void Heap::free(void* p)
{
	if (tryFree(p) != 0) throw "Can't free block at this address";
}

void* Heap::tryAllocate()
{
	if (freeSpace == 0) return nullptr;
	char* pos = (char*)memchr(status, 0, capacity);
	if (pos == nullptr) return nullptr;
	freeSpace--;
	*pos = 1;
	return buffer + (pos - status) * blockSize;
}

int Heap::tryFree(void* p)
{
	if (freeSpace == capacity) return 1;
	if (p < buffer) return 2;
	size_t ptr = (char*)p - buffer;
	size_t index = ptr / blockSize;
	if (index >= capacity) return 3;
	if (index * blockSize != ptr) return 4;
	if (status[index] == 0) return 5;
	freeSpace++;
	status[index] = 0;
	return 0;
}
