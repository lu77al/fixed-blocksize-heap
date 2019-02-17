#include "heap_fast.h"
#include <cstring>

#define EMPTY 0
#define OCCUPIED 1

HeapFast::HeapFast(void* memory, size_t memorySize, size_t blockSize)
{
	this->blockSize = blockSize;
	next = status = (char*)memory;
	freeSpace = capacity = memorySize / (blockSize + 1);
	if (capacity == 0) return;
	buffer = status + capacity;
	memset(status, EMPTY, capacity);
}

void* HeapFast::allocate()
{
	void* result = tryAllocate();
	if (result == nullptr) throw "Not enough space to allocate block";
	return result;
}

void HeapFast::free(void* p)
{
	if (tryFree(p) != 0) throw "Can't free block at this address";
}

void* HeapFast::tryAllocate()
{
	if (freeSpace == 0) return nullptr;
	if (*next != EMPTY) {
		char *pos = (char*)memchr(status, EMPTY, capacity);
		if (pos == nullptr) {
			freeSpace = 0;
			return nullptr;
		}
		next = pos;
	}
	void* result = buffer + (next - status) * blockSize;
	freeSpace--;
	*next = OCCUPIED;
	next++;
	if (next >= buffer) next = status;
	return result;
}

int HeapFast::tryFree(void* p)
{
	if (freeSpace == capacity) return 1;
	if (p < buffer) return 2;
	size_t ptr = (char*)p - buffer;
	size_t index = ptr / blockSize;
	if (index >= capacity) return 3;
	if (index * blockSize != ptr) return 4;
	if (status[index] == EMPTY) return 5;
	freeSpace++;
	status[index] = EMPTY;
	if (*next != EMPTY) next = &status[index];
	return 0;
}
