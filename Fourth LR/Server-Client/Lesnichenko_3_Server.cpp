#include <windows.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

int main()
{
	HANDLE handle_Pipe;
	string pipename, eventname;
	char *message = new char[256];
	OVERLAPPED overlapped;
	DWORD write;

	ZeroMemory(&overlapped, sizeof(overlapped)); // we take a memory block for the structure

	eventname = "Sashaevent";
	overlapped.hEvent = CreateEventA(NULL, TRUE, FALSE, eventname.c_str());

	pipename = "\\\\.\\pipe\\pepe";
	handle_Pipe = CreateNamedPipeA(pipename.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 256, 256, 0, NULL); // create and connecting to the named channel
	ConnectNamedPipe(handle_Pipe, &overlapped);

	cout << "enter message, but if you would to stop process, enter the \"exit\"\n\n";
	while (true)
	{
		cin >> message;
		if (strcmp(message, "exit") != 0)
		{
			cout << endl;
			WriteFile(handle_Pipe, message, strlen(message) + 1, &write, &overlapped); // put a message in channel
			WaitForSingleObject(overlapped.hEvent, INFINITE); // wait for I/O
		}
		else
		{
			cout << "\well, server finish...\n";
			break;
		}
	}

	DisconnectNamedPipe(handle_Pipe);

	return 0;
}
