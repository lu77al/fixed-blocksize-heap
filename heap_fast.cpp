#include "heap_fast.h"
#include <cstring>

#define EMPTY 0
#define OCCUPIED 1
#define ALIGNMENT 8
#define ST_ALIGNMENT sizeof(char*)

// - Calculates capacity of buffer (maximum blocks count) 
//   concedering memory alignment and sizes of state table and stack
// - Clears state table, setups other fields
// - If correct object can't be construced assumes capacity = 0
//   then no block will be allocated
HeapFast::HeapFast(void* memory, size_t memorySize, size_t blockSize)
{
	freeSpace = capacity = 0;     // Default values for "bad" object
	if (blockSize == 0) return;
	size_t alignedSize = ((blockSize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT; // Block size considering alignment
	size_t deltaSize = alignedSize - blockSize;	// delta to let last block have size = actualSize 
	capacity = (memorySize + deltaSize) / (alignedSize + 1 + sizeof(char*)) + 1; // First (optimistic) capacity +1
	size_t stackOffset, bufferOffset, bufferSize;
	do {	// Fit all structures correcting capacity
		capacity--;
		if (capacity == 0) return;
		stackOffset = ((capacity + ST_ALIGNMENT - 1) / ST_ALIGNMENT) * ST_ALIGNMENT;
		bufferOffset = ((stackOffset + capacity * sizeof(char*) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
		bufferSize = capacity * alignedSize - deltaSize;
	} while ((bufferOffset + bufferSize) > memorySize);
	status = (char*)memory;
	stack = (char**)(status + stackOffset);
	buffer = (char*)(status + bufferOffset);
	stackHead = stack - 1; // Stack is empty
	this->blockSize = alignedSize;
	freeSpace = capacity;
	initCnt = 0;
	memset(status, EMPTY, capacity); // Set all cells as empty
}

// Allocate or throw exception
void* HeapFast::allocate()
{
	void* result = tryAllocate();
	if (result == nullptr) throw "Not enough space to allocate block";
	return result;
}

// Free or throw exception
void HeapFast::free(void* p)
{
	if (tryFree(p) != 0) throw "Can't free block at this address";
}

// - Allocates block
// - returns pointer to allocated block or null if not enough space
void* HeapFast::tryAllocate()
{
	if (freeSpace == 0) return nullptr;
	char* pos;
	if (initCnt < capacity) // initially allocation order 0, 1, ... capacity-1
	{
		pos = &status[initCnt];
		initCnt++;
	}
	else // then pop cell address from the stack formed with calls of free()
	{
		if (stackHead < stack) return nullptr;
		pos = *stackHead;
		stackHead--;
	}
	if (*pos != EMPTY)  return nullptr; // unexpected error (maybe memory was corrupted)
	freeSpace--;
	*pos = OCCUPIED;
	return buffer + (pos - status) * blockSize;
}

// - Frees block
// - returns 0 if successfull or error code if fails
int HeapFast::tryFree(void* p)
{
	if (freeSpace == capacity) return 1;  // buffer is empty
	if (p < buffer) return 2;	// pointer is lower then buffer
	size_t ptr = (char*)p - buffer;
	size_t index = ptr / blockSize;
	if (index >= capacity) return 3; // pointer is higher then buffer
	if (index * blockSize != ptr) return 4; // bad alignment
	if (status[index] == EMPTY) return 5; // cell is already empty
	if (stackHead >= stack + capacity) return 6; // stack is corrupted
	freeSpace++;
	status[index] = EMPTY;
	stackHead++;	// Push cell address to stack to provide info for allocate()
	*stackHead = &status[index];
	return 0;
}
