#include <iostream>
#include <Windows.h>

using namespace std;

// https://studfile.net/preview/397644/page:38/ - возможно то что надо
// https://learn.microsoft.com/ru-ru/windows/win32/ipc/synchronous-and-overlapped-input-and-output
// https://learn.microsoft.com/ru-ru/windows/win32/fileio/file-buffering
// https://upread.ru/art.php?id=844


int callback = 0;
int operations;
int size_of_copyFile;

HANDLE firstHandle;
HANDLE secondHandle;

OVERLAPPED* over_1;
OVERLAPPED* over_2;

VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {

    callback++;

}

void copyFile(HANDLE firstHandle, HANDLE secondHandle, DWORD blockSize, int operationsCount) {

    int fileSize = GetFileSize(firstHandle, NULL);
    int initial_size = fileSize;
    int countOperations = 0;

    CHAR* buffer1 =  new CHAR[fileSize + (fileSize % blockSize > 0 ? 1 : 0) * blockSize];
    CHAR* buffer2 = new CHAR[fileSize + (fileSize % blockSize > 0 ? 1 : 0) * blockSize];

    OVERLAPPED* over_1 = new OVERLAPPED[operationsCount];
    OVERLAPPED* over_2 = new OVERLAPPED[operationsCount];


    for (int i = 0; i < operationsCount; i++)
    {

        over_1[i].Offset = 0;
        over_1[i].OffsetHigh = 0;
        over_1[i].hEvent = NULL;

        over_2[i].Offset = 0;
        over_2[i].OffsetHigh = 0;
        over_2[i].hEvent = NULL;

    }

    buffer2 = buffer1;
    bool inFlag = false;
    bool outFlag = false;

    do
    {
        callback = 0;
        countOperations = 0;

        for (int i = 0; i < operationsCount; i++)
        {
            if (fileSize > 0)
            {
                countOperations++;
                if (i || inFlag)
                {
                    for (int j = 0; j < operationsCount; j++) over_1[j].Offset += blockSize;
                    buffer1 += blockSize;
                }
                inFlag = true;
                ReadFileEx(firstHandle, buffer1, blockSize, &over_1[i], FileIOCompletionRoutine);
                fileSize -= blockSize;
            }

        }

        while (callback < countOperations)
            SleepEx(-1, TRUE);

        callback = 0;

        for (int i = 0; i < countOperations; i++)
        {
            if (i || outFlag)
            {
                for (int j = 0; j < operationsCount; j++)
                    over_2[j].Offset += blockSize;

                buffer2 += blockSize;
            }
            outFlag = true;
            WriteFileEx(secondHandle, buffer2, blockSize, &over_2[i], FileIOCompletionRoutine);
        }

        while (callback < countOperations)
            SleepEx(-1, TRUE);

    } while (fileSize > 0);

    SetFilePointer(secondHandle, initial_size, NULL, FILE_BEGIN);
    SetEndOfFile(secondHandle);

}



int main()
{
    int number = 0;
    int part = 0, operations = 0;
    const DWORD directoryBufferSize = MAX_PATH;
    wchar_t directoryBuffer[directoryBufferSize] = L"";
    const DWORD directoryOut = GetCurrentDirectoryW(directoryBufferSize, directoryBuffer);
    if (directoryOut > 0 && directoryOut <= directoryBufferSize)
    {

        cout <<"enter a multiplier for the block size: ";

        do {

            cin >> part;

        } while (part == 0);

        cout <<"entering the number of operations (1, 2 , 4, 8, 12 , 16): ";

        do {

            cin >> operations;

        } while (operations == 0);

        cout << "enter the first path: " << endl;

        wstring first_PATH = L"";
        wcin >> first_PATH;

        cout << "enter the second path: " << endl;
        wstring second_PATH = L"";
        wcin >> second_PATH;

        if (first_PATH.length() < MAX_PATH)
        {

            firstHandle = CreateFileW(first_PATH.data(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
                FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

            secondHandle = CreateFileW (second_PATH.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

            if (firstHandle != INVALID_HANDLE_VALUE)
            {
                size_of_copyFile = 4096 * part;

                DWORD startTime = GetTickCount();
                copyFile(firstHandle, secondHandle, size_of_copyFile, operations);
                DWORD endTime = GetTickCount();

                cout << endl << "operation time: " << endTime - startTime << endl;
                delete[] over_1, over_2;
            }
            else
            {
                if (GetLastError())
                    cout << endl << "error" << endl;
            }
        }

        if (!CloseHandle(firstHandle))
            cout << "can't close handler" << endl;

        if (!CloseHandle(secondHandle))
            cout << "can't close handler" << endl;
    }
    else
    {
        cout << "error";
    }
}
