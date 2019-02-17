#ifndef __HEAP__
#define __HEAP__

class Heap
{
public:
	Heap(void* memory, size_t memorySize, size_t blockSize);
	void* allocate();
	void free(void* p);
};

#endif