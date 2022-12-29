#include <windows.h>
#include <iostream>
#include <string>

#define PAGES 41 // 3 + 0 + 8 + 30 = 41

using std::string;
using std::to_string;
using std::cout;

HANDLE *SemaphoresWriters = new HANDLE[PAGES];
HANDLE *SemaphoresReaders = new HANDLE[PAGES];
HANDLE *ActiveProcess = new HANDLE[PAGES << 1];

void SetSemaphores(bool is_writer)
{
	string SemaName;
	for (int i = 0; i < PAGES; ++i)
	{
		if (is_writer)
		{
			SemaName = "semaphore " + to_string(i) + " (writer)";
			SemaphoresWriters[i] = CreateSemaphoreA(NULL, 1, 1, SemaName.c_str());
		}
		else
		{
			SemaName = "semaphore " + to_string(i) + " (reader)";
			SemaphoresReaders[i] = CreateSemaphoreA(NULL, 0, 1, SemaName.c_str());
		}
	}
}

void creating_processes(bool is_writer)
{
	LPSTARTUPINFOA startup_info = new STARTUPINFOA[PAGES];
	LPPROCESS_INFORMATION process_information = new PROCESS_INFORMATION[PAGES];
	string process_name;
	if (is_writer)
	{
		process_name = "D:\\OperationSystem\\test\\LesnichenkoWriter.exe";
	}
	else
	{
		process_name = "D:\\OperationSystem\\test\\LesnichenkoReader.exe";
	}
	for (int i = 0; i < PAGES; ++i)
	{
		ZeroMemory(&startup_info[i], sizeof(startup_info[i]));
		CreateProcessA(process_name.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &startup_info[i], &process_information[i]);
		if (is_writer)
		{
			ActiveProcess[i] = process_information[i].hProcess;
		}
		else
		{
			ActiveProcess[PAGES + i] = process_information[i].hProcess;
		}
	}
}

int main()
{

	SYSTEM_INFO system_info;
	int buffer_size;
	GetSystemInfo(&system_info);
	buffer_size = PAGES * system_info.dwPageSize;

	string buffer_name, mapped_name;
	HANDLE buffer_handle, mapped_handle;
	LPVOID address;

	buffer_name = "D:\\OperationSystem\\buffer.txt";
	mapped_name = "buffer";

	buffer_handle = CreateFileA(buffer_name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	mapped_handle = CreateFileMappingA(buffer_handle, NULL, PAGE_READWRITE, 0, buffer_size, mapped_name.c_str());
	address = MapViewOfFile(mapped_handle, FILE_MAP_WRITE, 0, 0, buffer_size);
	VirtualLock(address, buffer_size);

	SetSemaphores(true);
	SetSemaphores(false);

	creating_processes(true);
	creating_processes(false);

	WaitForMultipleObjects(PAGES << 1, ActiveProcess, TRUE, INFINITE);

	return 0;
}
