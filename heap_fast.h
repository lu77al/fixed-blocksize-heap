#ifndef __HEAP_FAST__
#define __HEAP_FAST__

class HeapFast
{
	char* status;
	char* buffer;
	size_t blockSize;
	size_t capacity;
	size_t freeSpace;
	char* next;

	void* tryAllocate();
	int tryFree(void* p);
public:
	HeapFast(void* memory, size_t memorySize, size_t blockSize);
	void* allocate();
	void free(void* p);
};

#endif