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
        SemaphoresReaders[i] = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SemaName.c_str());
    }
}

int main()
{

    SYSTEM_INFO system_info;
    int bufSize;
    GetSystemInfo(&system_info);
    bufSize = PAGES * system_info.dwPageSize;

    string bufName, mapName;
    HANDLE mapped_handle;
    LPVOID address;

    bufName = "D:\\OperationSystem\\buffer.txt";
    mapName = "buffer";

    // open the "projected file" object and project it onto the VAP of this process
    mapped_handle = OpenFileMappingA(GENERIC_WRITE, FALSE, mapName.c_str());
    address = MapViewOfFile(mapped_handle, FILE_MAP_WRITE, 0, 0, bufSize);

    //
    FILE *logfile, *logfile2;
    string logfile_name, logfile_name2;
    logfile_name = "D:\\OperationSystem\\writer log " + to_string(GetCurrentProcessId()) + ".txt";
    logfile = fopen(logfile_name.c_str(), "w");

    logfile_name2 = "D:\\OperationSystem\\writer log " + to_string(GetCurrentProcessId()) + ".txt";
    logfile2 = fopen(logfile_name2.c_str(), "w");

    discoverySemaphores();

    char *data = new char[4] { 'a', 'b', 'c', '\0'};

    DWORD start_time = timeGetTime();
    while (timeGetTime() < start_time + 15000)
    {
        fprintf(logfile, "|Process: %d|\t\t\t|State: WAITING|\t\t\t\t|time: %d|\n", GetCurrentProcessId(), timeGetTime());
        fprintf(logfile2, "%d %d\n", timeGetTime(), 0);

        DWORD page = WaitForMultipleObjects(PAGES, SemaphoresWriters, FALSE, INFINITE);

        fprintf(logfile, "|Process: %d|\t\t\t|State: WRITING; Page: %d|\t\t\t|time: %d|\n", GetCurrentProcessId(), page, timeGetTime());
        fprintf(logfile2, "%d %d\n", timeGetTime(), 1);

        int offset = page * system_info.dwPageSize; // offset relative to the beginning of buffer memory
        memcpy((LPVOID)((long long)address + offset), data, strlen(data) * sizeof(char));
        Sleep(500);

        string logname = "... " + to_string(page) + ".txt";
        FILE *log = fopen(logname.c_str(), "a");
        fprintf(log, "%d %d\n", timeGetTime(), 1);
        ReleaseSemaphore(SemaphoresReaders[page], 1, NULL);
        fprintf(log, "%d %d\n", timeGetTime(), 0);
        fclose(log);

        fprintf(logfile, "|Process: %d|\t\t\t|State: RELEASED|\t\t\t\t|time: %d|\n\n", GetCurrentProcessId(), timeGetTime());
        fprintf(logfile2, "%d %d\n", timeGetTime(), 2);
    }

    return 0;
}
