#include "heap.h"
#include <iostream>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;

void showDump(const char* header, const char* p, int n);
void showOffset(char* p[], char* base, int n);

int main(void)
{
	cout << "=== Heap class demo / test ===\n";
	cout << "\n1. Prepare guineapig memory region\n";
	char* mem = new char[200];
	memset(mem, 7, 200);
	showDump("dump: ", mem, 15);
	cout << "\n2. Create instance of Heap 200 / 15 bytes\n";
	Heap heap(mem, 200, 15);
	showDump("dump: ", mem, 15);
	cout << "\n3. Allocate 12 blocks\n";
	char* obj[12];
	for (int i = 0; i < 12; i++)
	{
		obj[i] = (char*)heap.allocate();
		showDump("alloction: ", mem, 15);
	}
	showOffset(obj, mem, 12);
	cout << "\n4. Free them all\n";
	for (int i = 0; i < 12; i++)
	{
		heap.free(obj[i]);
		showDump("alloction: ", mem, 15);
	}
	cin.get();
}

void showDump(const char* header, const char* p, int n)
{
	cout << header;
	for (int i = 0; i < n; i++)
	{
		cout << +p[i] << ", ";
	}
	cout << endl;
}

void showOffset(char* p[], char* base, int n)
{
	cout << "Blocks offset: ";
	for (int i = 0; i < n; i++)
	{
		cout << +(p[i] - base) << ((i < n - 1) ? ", " : "\n");
	}
}