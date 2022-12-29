#include <windows.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

void CALLBACK CompletionRoutine(DWORD errorCode, DWORD bytestransfered, LPOVERLAPPED lpOverlapped)
{
}

int main()
{
    cout << "hi! it's a your client!" << endl << endl;
	HANDLE handle_Pipe;
	string pipe_adress;
	char *message = new char[256];
	OVERLAPPED overlapped;

	ZeroMemory(&overlapped, sizeof(overlapped)); // we take a memory block for the structure

	pipe_adress = "\\\\.\\pipe\\pepe";
	handle_Pipe = CreateFileA(pipe_adress.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // connecting to the named channel

	BOOL result;
	while (true)
	{
		result = ReadFileEx(handle_Pipe, message, 256, &overlapped, CompletionRoutine); // read the message
		if (result && handle_Pipe != INVALID_HANDLE_VALUE) // successfully read
		{
			cout << "okay, we got a message: " << message << endl << endl;
			SleepEx(INFINITE, TRUE);
		}
		else
		{
			cout << "well, client exit\n";
			break;
		}
	}
	return 0;
}
