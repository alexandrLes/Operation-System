#include <iostream>
#include <Windows.h>
#include <tchar.h>

using namespace std;

int main() {
    // open the projected file
	HANDLE mapping_file = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"test1");

    // read the projected file
	if (mapping_file) {
		void* address = MapViewOfFile(mapping_file, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		cout << "mapped to address is " << address << endl;
		cout << "entered text is " << (char*)address << endl;

		UnmapViewOfFile(address);
		CloseHandle(mapping_file);
	}
	else
		cout << "error" << endl;
	return 0;
}
