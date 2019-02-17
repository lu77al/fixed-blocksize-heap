#include "heap.h"
#include "heap_fast.h"
#include <iostream>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;

void showDump(const char* header, const char* p, int n);
void showOffset(char* p[], char* base, int n);
void testHeap(void);
void testHeapFast(void);

int main(void)
{
	testHeap();
	testHeapFast();
}

void testHeapFast(void)
{
#define REGION_SIZE 255
#define BLOCK_SIZE 15

	cout << "=== HeapFast class demo / test ===\n";
	cout << "\n1. Prepare guineapig memory region\n";
	char* mem = new char[REGION_SIZE];
	memset(mem, 7, REGION_SIZE);
	showDump("dump: ", mem, 15);
	try {
		cout << "\n2. Create instance of Heap " << REGION_SIZE << " / 15 bytes\n";
		HeapFast heap(mem, REGION_SIZE, BLOCK_SIZE);
		showDump("dump: ", mem, 15);
		cout << "\n3. Allocate 12 blocks\n";
		char* obj[12];
		for (int i = 0; i < 12; i++)
		{
			obj[i] = (char*)heap.allocate();
			showDump("alloction: ", mem, 15);
		}
		showOffset(obj, mem, 12);
		cout << "\n4. Try to allocate more\n";
		try {
			heap.allocate();
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n5. Free obj[5], obj[11], obj[2], obj[6]\n";
		heap.free(obj[5]);
		heap.free(obj[11]);
		heap.free(obj[2]);
		heap.free(obj[6]);
		showDump("Status after free: ", mem, 15);
		cout << "\n6. Free the same twice\n";
		try {
			heap.free(obj[2]);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n7. Free before memory region\n";
		try {
			heap.free(mem - 10);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n8. Free after memory region\n";
		try {
			heap.free(obj[10] + 5 * 15);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n9. Free with incorrect alignment\n";
		try {
			heap.free(obj[3] + 1);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		showDump("Status is the same: ", mem, 15);
		cout << "\n10. Reallocate obj[6], obj[2], obj[11], obj[5]\n";
		obj[6] = (char*)heap.allocate();
		obj[2] = (char*)heap.allocate();
		obj[11] = (char*)heap.allocate();
		obj[5] = (char*)heap.allocate();
		showDump("Status: ", mem, 15);
		showOffset(obj, mem, 12);
		cout << "\n11. Free them all\n";
		for (int i = 0; i < 12; i++)
		{
			heap.free(obj[i]);
			showDump("Free: ", mem, 15);
		}
		cout << "\n12. Locate all again\n";
		for (int i = 0; i < 12; i++)
		{
			obj[i] = (char*)heap.allocate();
			showDump("allocation: ", mem, 15);
		}
		showOffset(obj, mem, 12);
	}
	catch (const char* msg)
	{
		cout << "\nERROR!!! " << msg << endl;
	}
	catch (const int msg)
	{
		cout << "\nERROR!!! Exception was expected\n";
	}

	cout << "\n*** Ok, no unexpected exceptions were thrown ***\n";

	delete[] mem;
	cin.get();
}

void testHeap(void)
{
	#define REGION_SIZE 207
	#define BLOCK_SIZE 15

	cout << "=== Heap class demo / test ===\n";
	cout << "\n1. Prepare guineapig memory region\n";
	char* mem = new char[REGION_SIZE];
	memset(mem, 7, REGION_SIZE);
	showDump("dump: ", mem, 15);
	try {
		cout << "\n2. Create instance of Heap " << REGION_SIZE << " / 15 bytes\n";
		Heap heap(mem, REGION_SIZE, BLOCK_SIZE);
		showDump("dump: ", mem, 15);
		cout << "\n3. Allocate 12 blocks\n";
		char* obj[12];
		for (int i = 0; i < 12; i++)
		{
			obj[i] = (char*)heap.allocate();
			showDump("alloction: ", mem, 15);
		}
		showOffset(obj, mem, 12);
		cout << "\n4. Try to allocate more\n";
		try {
			heap.allocate();
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n5. Free obj[5], obj[11], obj[2], obj[6]\n";
		heap.free(obj[5]);
		heap.free(obj[11]);
		heap.free(obj[2]);
		heap.free(obj[6]);
		showDump("Status after free: ", mem, 15);
		cout << "\n6. Free the same twice\n";
		try {
			heap.free(obj[2]);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n7. Free before memory region\n";
		try {
			heap.free(mem - 10);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n8. Free after memory region\n";
		try {
			heap.free(obj[10] + 5 * 15);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		cout << "\n9. Free with incorrect alignment\n";
		try {
			heap.free(obj[3] + 1);
			throw 1;
		}
		catch (const char* msg)
		{
			cout << "Expected exception Ok: " << msg << endl;
		}
		showDump("Status is the same: ", mem, 15);
		cout << "\n10. Reallocate obj[5], obj[11], obj[2], obj[6]\n";
		obj[5] = (char*)heap.allocate();
		obj[11] = (char*)heap.allocate();
		obj[2] = (char*)heap.allocate();
		obj[6] = (char*)heap.allocate();
		showDump("Status: ", mem, 15);
		showOffset(obj, mem, 12);
		cout << "\n11. Free them all\n";
		for (int i = 0; i < 12; i++)
		{
			heap.free(obj[i]);
			showDump("Free: ", mem, 15);
		}
	}
	catch (const char* msg)
	{
		cout << "\nERROR!!! " << msg << endl;
	}
	catch (const int msg)
	{
		cout << "\nERROR!!! Exception was expected\n";
	}

	cout << "\n*** Ok, no unexpected exceptions were thrown ***\n";

	delete[] mem;
	cin.get();
}


void showDump(const char* header, const char* p, int n)
{
	cout << header;
	for (int i = 0; i < n; i++)
	{
		cout << +p[i] << ", ";
	}
	cout << "...\n";
}

void showOffset(char* p[], char* base, int n)
{
	cout << "Blocks offset: ";
	for (int i = 0; i < n; i++)
	{
		cout << +(p[i] - base) << ((i < n - 1) ? ", " : "\n");
	}
}