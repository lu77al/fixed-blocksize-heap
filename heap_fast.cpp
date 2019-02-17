#include "heap_fast.h"
#include <cstring>

#define EMPTY 0
#define OCCUPIED 1
#define ALIGNMENT 8

HeapFast::HeapFast(void* memory, size_t memorySize, size_t blockSize)
{
	freeSpace = capacity = 0;     // Default values for "bad" object
	size_t alignedSize = ((blockSize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT; // Block size considering alignment
	size_t deltaSize = alignedSize - blockSize;	// delta to let last block have size = actualSize 
	capacity = (memorySize + deltaSize) / (blockSize + 1 + sizeof(char*)) + 1;
	size_t stateSize, stackSize, bufferSize;
	do {
		capacity--;
		if (capacity == 0) return;
		stateSize = ((capacity + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
		stackSize = ((capacity * sizeof(char*) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
		bufferSize = capacity * blockSize - deltaSize;
	} while ((stateSize + stackSize + bufferSize) > memorySize);
	status = (char*)memory;
	stack = (char**)(status + stateSize);
	buffer = (char*)(status + stateSize + stackSize);
	stackHead = stack - 1;
	this->blockSize = alignedSize;
	freeSpace = capacity;
	initCnt = 0;
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
	char* pos;
	if (initCnt < capacity)
	{
		pos = &status[initCnt];
		initCnt++;
	}
	else
	{
		if (stackHead < stack) return nullptr;
		pos = *stackHead;
		stackHead--;
	}
	if (*pos != EMPTY)  return nullptr;
	freeSpace--;
	*pos = OCCUPIED;
	return buffer + (pos - status) * blockSize;
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
*/