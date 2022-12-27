#include <iostream>
#include <Windows.h>
#include <tchar.h>

using namespace std;

int main() {
    // creating a text file
	HANDLE file = CreateFile(L"D:\\test.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// create the projected file
	HANDLE mapping_file = CreateFileMapping(file, NULL, PAGE_READWRITE, 0, 100, L"test1");

	void*  address = MapViewOfFile(mapping_file, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	cout << "mapped to address is " << address << endl;

	cout << "enter the text: ";
	cin.getline((char*)address, 100);
	system("pause");

	UnmapViewOfFile(address);
	CloseHandle(file);
	CloseHandle(mapping_file);
	return 0;
}
