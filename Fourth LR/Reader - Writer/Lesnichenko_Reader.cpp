#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>

#define PAGES 41 // 3 + 0 + 8 + 30 = 41

using std::string;
using std::to_string;
using std::strlen;

HANDLE *SemaphoresWriters = new HANDLE[PAGES];
HANDLE *SemaphoresReaders = new HANDLE[PAGES];

void discoverySemaphores()
{
    string SemaName;
    for (int i = 0; i < PAGES; ++i)
    {
        SemaName = "semaphore " + to_string(i) + " (writer)";
        SemaphoresWriters[i] = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SemaName.c_str());
        SemaName = "semaphore " + to_string(i) + " (reader)";
        SemaphoresReaders[i] = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SemaName.c_str());
    }
}

void log_page(DWORD page)
{
    string logname = "D:\\OS4\\pages for excel\\page " + to_string(page) + ".txt";
    FILE *log = fopen(logname.c_str(), "w");

}

int main()
{
    srand(time(NULL));
    SYSTEM_INFO system_info;
    int buffer_size;
    GetSystemInfo(&system_info);
    buffer_size = PAGES * system_info.dwPageSize;

    string buffer_name, mapped_name;
    HANDLE mapped_handle;
    LPVOID address;

    buffer_name = "D:\\OS4\\buffer.txt";
    mapped_name = "buffer";

    mapped_handle = OpenFileMappingA(GENERIC_READ, FALSE, mapped_name.c_str());
    address = MapViewOfFile(mapped_handle, FILE_MAP_READ, 0, 0, buffer_size);

    FILE *logfile, *logfile2;
    string logfile_name, logfile_name2;
    logfile_name = "D:\\OperationSystem\\reader log " + to_string(GetCurrentProcessId()) + ".txt";
    logfile = fopen(logfile_name.c_str(), "w");

    logfile_name2 = "D:\\OperationSystem\\reader log " + to_string(GetCurrentProcessId()) + ".txt";
    logfile2 = fopen(logfile_name2.c_str(), "w");

    discoverySemaphores();

    char *data;

    DWORD start_time = timeGetTime();
    while (timeGetTime() < start_time + 15000)
    {
        fprintf(logfile, "|Process: %d|\t\t\t|State: WAITING|\t\t\t\t\t|time: %d|\n", GetCurrentProcessId(), timeGetTime());
        fprintf(logfile2, "%d %d\n", timeGetTime(), 0);

        DWORD page = WaitForMultipleObjects(PAGES, SemaphoresReaders, FALSE, INFINITE);

        fprintf(logfile, "|Process: %d|\t\t\t|State: READING; Page: %d|\t\t\t\t|time: %d|\n", GetCurrentProcessId(), page, timeGetTime());
        fprintf(logfile2, "%d %d\n", timeGetTime(), 1);

        int offset = page * system_info.dwPageSize;// offset relative to the beginning of buffer memory
        data = (char*)((long long)address + offset);
        Sleep(500 + rand() % 1001);

        string logname = "... " + to_string(page) + ".txt";
        FILE *log = fopen(logname.c_str(), "a");
        fprintf(log, "%d %d\n", timeGetTime(), 2);
        ReleaseSemaphore(SemaphoresWriters[page], 1, NULL);
        fprintf(log, "%d %d\n", timeGetTime(), 0);
        fclose(log);

        fprintf(logfile, "|Process: %d|\t\t\t|State: RELEASED; Read data: %s|\t\t\t|time: %d|\n\n", GetCurrentProcessId(), data, timeGetTime());
        fprintf(logfile2, "%d %d\n", timeGetTime(), 2);
    }

    return 0;
}
