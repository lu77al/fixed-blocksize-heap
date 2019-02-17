#ifndef __HEAP_FAST__
#define __HEAP_FAST__

class HeapFast
{
	char* status; // status table 1 byte per cell (EMPTY / OCCUPIED). Placed at start of pre-allocted region
	char** stack; // stack containing pointers to status table cells after free(). To get rid of search for empty cell. Placed after status table
	char** stackHead; 
	char* buffer; // buffer with allocated blocks
	size_t blockSize; // block size considering alignment
	size_t capacity;  // total cells count (maximum blocks available)
	size_t initCnt;   // coutnter of initial allocations
	size_t freeSpace; // empty cells count 
	
	void* tryAllocate();
	int tryFree(void* p);
public:
	HeapFast(void* memory, size_t memorySize, size_t blockSize);
	void* allocate();
	void free(void* p);
};

#endif