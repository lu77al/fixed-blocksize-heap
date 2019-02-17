#ifndef __HEAP__
#define __HEAP__

class Heap
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
	Heap(void* memory, size_t memorySize, size_t blockSize);
	void* allocate();
	void free(void* p);
};

#endif