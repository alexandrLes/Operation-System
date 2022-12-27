#include <iostream>
#include <locale.h>
#include <windows.h>
#include <cstring>

using namespace std;

void menu() {
	cout << "Processor: Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz, 2197 MHz, Cores: 2, Logical processors: 4\n"
        "Manufacturer: ASUSTeK COMPUTER INC.\n\n"
		"1: Get system information\n"
		"2: Get memory status\n"
		"3: State of memory region\n"
		"4: Reserve memory\n"
		"5: Reserve and allocation physical memory\n"
		"6: Writing data to memory cells\n"
		"7: Setting access protection for a specified memory region\n"
		"8: Freeing physical memory and memory region\n"
		"0: exit\n\n";
}

void ComputerSystemInfo(int attr) {
    // We determine the type of processor.

	if (attr == PROCESSOR_ARCHITECTURE_AMD64) cout << "x64 (AMD or Intel)" << endl;
	if (attr == PROCESSOR_ARCHITECTURE_IA64) cout << "Intel Itanium Processor Family" << endl;
	if (attr == PROCESSOR_ARCHITECTURE_INTEL) cout << "x86 Intel" << endl;
	if (attr == PROCESSOR_ARCHITECTURE_UNKNOWN) cout << "Unknown" << endl;
}

void ProcessorFeature() {
    // Determines whether the specified processor component is supported by the current computer.

	if (IsProcessorFeaturePresent(0)) cout << "\tOn a Pentium, a floating-point precision error can occur in rare circumstances" << endl;
	if (IsProcessorFeaturePresent(1)) cout << "\tFloating-point operations are emulated using a software emulator" << endl;
	if (IsProcessorFeaturePresent(2)) cout << "\tThe atomic compare and exchange operation (cmpxchg) is available" << endl;
	if (IsProcessorFeaturePresent(3)) cout << "\tThe MMX instruction set is available" << endl;
	if (IsProcessorFeaturePresent(6)) cout << "\tThe SSE instruction set is available" << endl;
	if (IsProcessorFeaturePresent(7)) cout << "\tThe 3D-Now instruction set is available" << endl;
	if (IsProcessorFeaturePresent(8)) cout << "\tThe RDTSC instruction is available" << endl;
	if (IsProcessorFeaturePresent(9)) cout << "\tThe processor is PAE-enabled" << endl;
	if (IsProcessorFeaturePresent(10)) cout << "\tThe SSE2 instruction set is available" << endl;
	if (IsProcessorFeaturePresent(12)) cout << "\tData execution prevention is enabled." << endl;
	if (IsProcessorFeaturePresent(13)) cout << "\tThe SSE3 instruction set is available" << endl;
	if (IsProcessorFeaturePresent(14)) cout << "\tThe atomic compare and exchange 128-bit operation (cmpxchg16b) is available" << endl;
	if (IsProcessorFeaturePresent(15)) cout << "\tThe atomic compare 64 and exchange 128-bit operation (cmp8xchg16) is available" << endl;
	if (IsProcessorFeaturePresent(16)) cout << "\tThe processor channels are enabled" << endl;
	if (IsProcessorFeaturePresent(17)) cout << "\tThe processor implements the XSAVE and XRSTOR instructions" << endl;
	if (IsProcessorFeaturePresent(18)) cout << "\tThe VFP/Neon: 32 x 64bit register bank is present" << endl;
	if (IsProcessorFeaturePresent(20)) cout << "\tSecond Level Address Translation is supported by the hardware" << endl;
	if (IsProcessorFeaturePresent(21)) cout << "\tVirtualization is enabled in the firmware and made available by the operating system" << endl;
	if (IsProcessorFeaturePresent(22)) cout << "\tRDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE instructions are available" << endl;
	if (IsProcessorFeaturePresent(23)) cout << "\t_fastfail() is available" << endl;
	if (IsProcessorFeaturePresent(24)) cout << "\tThe divide instructions are available" << endl;
	if (IsProcessorFeaturePresent(25)) cout << "\tThe 64-bit load/store atomic instructions are available" << endl;
	if (IsProcessorFeaturePresent(26)) cout << "\tThe external cache is available" << endl;
	if (IsProcessorFeaturePresent(27)) cout << "\tThe floating-point multiply-accumulate instruction is available" << endl;
	if (IsProcessorFeaturePresent(29)) cout << "\tThis ARM processor implements the the ARM v8 instructions set" << endl;
	if (IsProcessorFeaturePresent(30)) cout << "\tThis ARM processor implements the ARM v8 extra cryptographic instructions" << endl;
	if (IsProcessorFeaturePresent(31)) cout << "\tThis ARM processor implements the ARM v8 extra CRC32 instructions" << endl;
	if (IsProcessorFeaturePresent(34)) cout << "\tThis ARM processor implements the ARM v8.1 atomic instructions" << endl;
}

void getCompSystemInfo() {
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);

	cout << "Hardware information: " << endl;
	cout << "Architecture of processor: ";
	ComputerSystemInfo(siSysInfo.wProcessorArchitecture);
	cout << "Page size: " << siSysInfo.dwPageSize << endl;
	cout << "Minimum application address: " << siSysInfo.lpMinimumApplicationAddress << endl;
	cout << "Maximum application address: " << siSysInfo.lpMaximumApplicationAddress << endl;
	cout << "Active processor mask: " << siSysInfo.dwActiveProcessorMask << endl;
	cout << "Number of processors: " << siSysInfo.dwNumberOfProcessors << endl;
	cout << "Processor type: " << siSysInfo.dwProcessorType << endl;
	cout << "Granularity for starting address: " << siSysInfo.dwAllocationGranularity << endl;
	cout << "Processor level: " << siSysInfo.wProcessorLevel << endl;
	cout << "Processor features: " << endl;
	ProcessorFeature();
}

void memoryStatus() {
    // information about the current state of physical and virtual memory, including extended memory.

	MEMORYSTATUSEX memStatus;
	memStatus.dwLength = sizeof(memStatus);
	GlobalMemoryStatusEx(&memStatus);

	cout << "Memory information: " << endl;
	cout << "Physical memory used: " << memStatus.dwMemoryLoad << "%" << endl;
	cout << "Total physical memory in bytes: " << memStatus.ullTotalPhys << endl;
	cout << "Available physical memory in bytes: " << memStatus.ullAvailPhys << endl;
	cout << "Paging file size in bytes: " << memStatus.ullTotalPageFile << endl;
	cout << "Available bytes in the paging file: " << memStatus.ullAvailPageFile << endl;
	cout << "Total virtual memory in bytes: " << memStatus.ullTotalVirtual << endl;
	cout << "Available virtual memory in bytes: " << memStatus.ullAvailVirtual << endl;
}

void pageState(int adress) {
	if (adress == MEM_COMMIT) cout << "Committed" << endl;
	if (adress == MEM_RESERVE) cout << "Reserved" << endl;
	if (adress == MEM_FREE) cout << "Free" << endl;
}

void pageType(int type) {
    // now we determine the level of protection

	if (type == MEM_PRIVATE)
		cout << "Private" << endl;
	else if (type == MEM_MAPPED)
		cout << "Section" << endl;
	else if (type == MEM_IMAGE)
		cout << "Image section" << endl;
	else cout << type << endl;         ///?
}

void accessProtection(int type) {
    // we determine the current level of protection

	if (type == PAGE_NOACCESS) cout << "PAGE_NOACCESS" << endl;
	if (type == PAGE_READONLY) cout << "PAGE_READONLY" << endl;
	if (type == PAGE_READWRITE) cout << "PAGE_READWRITE" << endl;
	if (type == PAGE_EXECUTE) cout << "PAGE_EXECUTE" << endl;
	if (type == PAGE_EXECUTE_READ) cout << "PAGE_EXECUTE_READ" << endl;
	if (type == PAGE_EXECUTE_READWRITE) cout << "PAGE_EXECUTE_READWRITE" << endl;
}

void memoryadress(long lpAddress) {
	MEMORY_BASIC_INFORMATION memStatus;
	VirtualQuery((LPCVOID)lpAddress, &memStatus, sizeof(memStatus));

	cout << "Base address of region: " << memStatus.BaseAddress << endl;
	cout << "Allocation base address: " << memStatus.AllocationBase << endl;
	cout << "Initial access protection: " << memStatus.AllocationProtect << endl;
	cout << "Region size in bytes:" << memStatus.RegionSize << endl;
	cout << "adress of pages in region: ";
	pageState(memStatus.State);
	cout << "Current access protection: ";
	accessProtection(memStatus.Protect);
	cout << "Type of pages: ";
	pageType(memStatus.Type);
}

void memoryReserve(int answer) {
	long lpAddress;
	void* memory;
	if (answer & 1) {
		memory = VirtualAlloc(NULL, 4096, MEM_RESERVE, PAGE_READWRITE);
	}
	else
	{
		cout << "Enter the address of the memory region: ";
		cin >> hex >> lpAddress;
		memory = VirtualAlloc((void*)lpAddress, 4096, MEM_RESERVE, PAGE_READWRITE);
	}
	if (memory)
		cout << "adress: " << memory << endl;
	else
		cout << "Error" << endl;
}

void memoryReserveBen(int answer) {
	long lpAddress;
	void* memory;
	if (answer & 1) {
		memory = VirtualAlloc(NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	}
	else
	{
		cout << "Enter the address of the memory region: ";
		cin >> hex >> lpAddress;
		memory = VirtualAlloc((void*)lpAddress, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	}
	if (memory)
		cout << "adress: " << memory << endl;
	else
		cout << "Error" << endl;
}

void memoryFilling(long lpAddress) {
	void* memory;

	memory = VirtualAlloc((void*)lpAddress, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	cout << "Memory allocation: ";
	if (memory)
		cout << "Success" << endl;
	else
		cout << "Error" << endl;
	getchar();
	fflush(stdin);

	cout << "Enter the text, max 100 symbols: ";
	cin.getline((char*)lpAddress, 100);
	cout << "Entered text: " << (char*)lpAddress << endl;
}

void memoryProtection(long lpAddress) {
	MEMORY_BASIC_INFORMATION memStatus;
	DWORD flNewProtect,
		lpflOldProtect;
	int protection;
	void* memory;

	memory = VirtualAlloc((void*)lpAddress, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	cout << "Memory allocation: ";
	if (memory)
		cout << "Success" << endl;
	else
		cout << "Error" << endl;

	VirtualQuery((LPCVOID)lpAddress, &memStatus, sizeof(memStatus));
	cout << "Current access protection: ";
	accessProtection(memStatus.Protect);

	cout << "Change to: " << endl;
	cout << "1-PAGE_NOACCESS" << endl;
	cout << "2-PAGE_READONLY" << endl;
	cout << "3-PAGE_READWRITE" << endl;
	cout << "4-PAGE_EXECUTE" << endl;
	cout << "5-PAGE_EXECUTE_READ" << endl;
	cout << "6-PAGE_EXECUTE_READWRITE" << endl;
	cout << endl;

	cin >> protection;
	if (protection == 1) flNewProtect = PAGE_NOACCESS;
	if (protection == 2) flNewProtect = PAGE_READONLY;
	if (protection == 3) flNewProtect = PAGE_READWRITE;
	if (protection == 4) flNewProtect = PAGE_EXECUTE;
	if (protection == 5) flNewProtect = PAGE_EXECUTE_READ;
	if (protection == 6) flNewProtect = PAGE_EXECUTE_READWRITE;
	cout << "Change protection: ";
	if (VirtualProtect((void*)lpAddress, 4096, flNewProtect, &lpflOldProtect))
		cout << "Success" << endl;
	else
		cout << "Error" << endl;

	VirtualQuery((LPCVOID)lpAddress, &memStatus, sizeof(memStatus));
	cout << "Current access protection: ";
	accessProtection(memStatus.Protect);
}

void freeMemorySystem(long lpAddress) {
	void* memory;
	if (!VirtualFree((void*)lpAddress, 0, MEM_RELEASE)) {
		memory = VirtualAlloc((void*)lpAddress, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		cout << "Memory allocation: ";
		if (memory)
			cout << "Success" << endl;
		else
			cout << "Error" << endl;

		cout << "Memory freeing: ";
		if (VirtualFree((void*)lpAddress, 0, MEM_RELEASE))
			cout << "Success" << endl;
		else
			cout << "Error" << endl;
	}
	else
		cout << "Memory freeing: Success" << endl;
}

void answers(int user_ans) {
	long lpAddress;
	int mode_answer;

	switch (user_ans) {
	case 1:
		getCompSystemInfo();
		break;
	case 2:
		memoryStatus();
		break;
	case 3:
		cout << "Enter the address of the memory region: ";
		cin >> hex >> lpAddress;
		memoryState(lpAddress);
		break;
	case 4:
		cout << "1.. Reserve in automatic mode\n"
			"2-Reserve in address input mode\n" << endl;
		cin >> mode_answer;
		memoryReserve(mode_answer);
		break;
	case 5:
		cout << "1.. Reserve and allocation in auto mode\n"
			"2-Reserve and allocation in address input mode\n" << endl;
		cin >> mode_answer;
		memoryReserveBen(mode_answer);
		break;
	case 6:
		cout << "Enter the address of the memory region (hex): ";
		cin >> hex >> lpAddress;
		memoryFilling(lpAddress);
		break;
	case 7:
		cout << "Enter the address of the memory region (hex): ";
		cin >> hex >> lpAddress;
		memoryProtection(lpAddress);
		break;
	case 8:
		cout << "Enter the address of the memory region (hex): ";
		cin >> hex >> lpAddress;
		freeMemorySystem(lpAddress);
		break;
	default:
		break;
	}

	system("pause");
	system("cls");
}

int main() {
	int user_ans;
	do {
		menu();
		cin >> user_ans;
		answers(user_ans);
	} while (user_ans != 0);

	return 0;
}
