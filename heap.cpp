#include "heap.h"
#include <cstring>

#define EMPTY 0
#define OCCUPIED 1
#define ALIGNMENT 8

// - Calculates capacity of buffer (maximum blocks count) 
//   concedering memory alignment and size of state table
// - Clears state table, setups other fields
// - If correct object can't be construced assumes capacity = 0
//   then no block will be allocated
Heap::Heap(void* memory, size_t memorySize, size_t blockSize)
{
	freeSpace = capacity = 0;     // Default values for "bad" object
	if (blockSize == 0) return;
	size_t actualSize = blockSize;
	blockSize = ((blockSize + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT; // Block size considering alignment
	size_t deltaSize = blockSize - actualSize;	// delta to let last block have size = actualSize 
	this->blockSize = blockSize;
	capacity = (memorySize + deltaSize) / (blockSize + 1); // Capacity (first iteration) 
	if (capacity == 0) return;
	size_t bufferOffset = ((capacity + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT; // Offset of first block considering alignment
	capacity = bufferOffset < memorySize ? (memorySize + deltaSize - bufferOffset) / blockSize : 0; // Capacity considering alignment
	if (capacity == 0) return;
	freeSpace = capacity;
	next = status = (char*)memory;
	buffer = status + bufferOffset;
	memset(status, EMPTY, capacity); // Set all cells as empty
}

// Allocate or throw exception
void* Heap::allocate()
{
	void* result = tryAllocate();
	if (result == nullptr) throw "Not enough space to allocate block";
	return result;
}

// Free or throw exception
void Heap::free(void* p)
{
	if (tryFree(p) != 0) throw "Can't free block at this address";
}

// - Allocates block
// - returns pointer to allocated block or null if not enough space
void* Heap::tryAllocate()
{
	if (freeSpace == 0) return nullptr;
	if (*next != EMPTY) { // if next isn't empty try to find empty cell
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
	if (next >= status + capacity) next = status;
	return result;
}

// - Frees block
// - returns 0 if successfull or error code if fails
int Heap::tryFree(void* p)
{
	if (freeSpace == capacity) return 1; // buffer is empty
	if (p < buffer) return 2;	// pointer is lower then buffer
	size_t ptr = (char*)p - buffer;
	size_t index = ptr / blockSize;
	if (index >= capacity) return 3; // pointer is higher then buffer
	if (index * blockSize != ptr) return 4; // bad alignment
	if (status[index] == EMPTY) return 5; // cell is already empty
	freeSpace++;
	status[index] = EMPTY;
	if (*next != EMPTY) next = &status[index]; // update next to allocate next time without search
	return 0;
}
