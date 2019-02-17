#ifndef __HEAP__
#define __HEAP__

class Heap
{
	char* status; // status table 1 byte per cell (EMPTY / OCCUPIED). Placed at start of pre-allocted region
	char* buffer; // buffer with allocated blocks
	size_t blockSize; // block size considering alignment
	size_t capacity;  // total cells count (maximum blocks available)
	size_t freeSpace; // empty cells count 
	char* next; // pointer (inside status table) to allocate next block

	void* tryAllocate();
	int tryFree(void* p);
public:
	Heap(void* memory, size_t memorySize, size_t blockSize);
	void* allocate();
	void free(void* p);
};

#endif