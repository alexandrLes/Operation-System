#include <iostream>
#include <locale.h>
#include <windows.h>

using namespace std;

void out_menu() {
    cout << "1: output a list of disks" << endl;
    cout << "2: output of information about the disk and the size of free space" << endl;
    cout << "3: creating and deleting specified directories" << endl;
    cout << "4: creation in the specified directory" << endl;
    cout << "5: copying and moving files between directories" << endl;
    cout << "6: analyzing and changing file attributes" << endl;
    cout << "0: exit" << endl;
}

void out_attr() {
    cout << "1: output of information about attributes" << endl;
    cout << "2: changing attribute information" << endl;
    cout << "3: output of file information by descriptor" << endl;
    cout << "4: system time information" << endl;
    cout << "5: changing the system time" << endl;
}

int main() {
	system("chcp 1251");

	int menu, temp, n;

	string name_of_disk;
	wstring w_string;

    LPCWSTR file_name;
	LPCWSTR name;
	LPCWSTR direct_name;

	UINT x;

	WCHAR lpVolumeNamebuffer1[MAX_PATH + 1];
	DWORD lpVolumeSerialNumber = 0;
	DWORD lpMaximumComponentLength = 0;
	DWORD lpFileSystemFlags = 0;
	WCHAR lpFileSystemNamebuffer1[MAX_PATH + 1];

	DWORD sectorsPerCluster;
	DWORD bytesPerSector;
	DWORD numberOfFreeClusters;
	DWORD totalNumberOfClusters;

	string name_of_disk2;

	wstring w_string_2;

	LPCWSTR firstFile;
	LPCWSTR secondFile;

	int moveFileEx_temp;

	DWORD attribute;
	DWORD setAttr;

	LPCSTR fileLPC;

	BY_HANDLE_FILE_INFORMATION lpFileInformation;

	HANDLE fileT;

	FILETIME timeCreate, timeLastAccess, timeLastWrite;
	FILETIME fileTime;

	SYSTEMTIME time;
	SYSTEMTIME systemTime;

	do {

		system("cls");
		out_menu();

		cin >> menu;
		system("cls");

		switch (menu) {

		case 1:

            // Решение первого пункта - Вывод списка доступных дисков, используя функцию GetLogicalDrives()

			cout << "disks: " << endl;

			temp = GetLogicalDrives();

			for (int i = 0; i < 26; i++)
			{
				n = ((temp >> i) & 1);
				if (n)
				{
					cout << "Disk: " << (char)(65 + i) << endl;
				}
			}

			system("pause");
			break;

		case 2:

			// Решение второго пункта - Вывод информации о диске, используя функцию GetDriveTypeW()

			cout << "Enter Disk Name (Example: d:\\): ";
			cin >> name_of_disk;

			w_string = wstring(name_of_disk.begin(), name_of_disk.end());
			name = w_string.c_str();
			x = GetDriveTypeW(name);

			if (x == DRIVE_UNKNOWN) cout <<     "- Unknown Type" << endl;
			if (x == DRIVE_NO_ROOT_DIR) cout << "- Error Path" << endl;
			if (x == DRIVE_REMOVABLE) cout <<   "- Removable Disk" << endl;
			if (x == DRIVE_FIXED) cout <<       "- Fixed Disk" << endl;
			if (x == DRIVE_REMOTE) cout <<      "- Remote disk)" << endl;
			if (x == DRIVE_CDROM) cout <<       "- CDROM Disk" << endl;
			if (x == DRIVE_RAMDISK) cout <<     "- RAMDISK Disk" << endl;


			// GetVolumeInfomation - В случае ошибки функция возвращает 0

			if (!GetVolumeInformationW(name, lpVolumeNamebuffer1, sizeof(lpVolumeNamebuffer1), &lpVolumeSerialNumber,
									 &lpMaximumComponentLength, &lpFileSystemFlags,
									 lpFileSystemNamebuffer1, sizeof(lpFileSystemNamebuffer1)))
			{
				cout << "Error" << endl;
			}
			else {
				cout << endl << "Name of the specified disk: " << lpVolumeNamebuffer1 << endl;
				cout << "Disk serial number: " << hex << lpVolumeSerialNumber << endl;
				cout << "Maximum file length: " << dec << lpMaximumComponentLength << endl;
				cout << "File System Options: " << endl;

				cout << "--- ... --- ... --- ..." << endl;


				if (lpFileSystemFlags & FILE_CASE_PRESERVED_NAMES)
					cout << " .. the specified volume maintains a saved file name register when it puts the name on disk." << endl;
				if (lpFileSystemFlags & FILE_CASE_SENSITIVE_SEARCH)
					cout << " .. the specified volume supports case-sensitive file names." << endl;
				if (lpFileSystemFlags & 0x20000000)
					cout << " .. the specified volume is a Direct Access volume (DAX)." << endl;
				if (lpFileSystemFlags & FILE_FILE_COMPRESSION)
					cout << " .. the specified volume supports file compression." << endl;
				if (lpFileSystemFlags & FILE_NAMED_STREAMS)
					cout << " .. the specified volume supports named threads." << endl;
				if (lpFileSystemFlags & FILE_PERSISTENT_ACLS)
					cout << " .. the specified volume stores and applies access control lists (ACLs)." << endl;
				if (lpFileSystemFlags & FILE_READ_ONLY_VOLUME)
					cout << " .. the specified volume is read-only." << endl;
				if (lpFileSystemFlags & FILE_SEQUENTIAL_WRITE_ONCE)
					cout << " .. The specified volume supports one sequential write." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_ENCRYPTION)
					cout << " .. The specified volume supports an encrypted file system (EFS)." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES)
					cout << " .. The specified volume supports extended attributes." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_HARD_LINKS)
					cout << " .. The specified volume supports hard links." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_OBJECT_IDS)
					cout << " .. The specified volume supports object identifiers." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID)
					cout << " .. The file system supports opening by FileID." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_REPARSE_POINTS)
					cout << " .. The specified volume supports re-parsing points." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_SPARSE_FILES)
					cout << " .. The specified volume supports sparse files." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_TRANSACTIONS)
					cout << " .. The specified volume supports transactions." << endl;
				if (lpFileSystemFlags & FILE_SUPPORTS_USN_JOURNAL)
					cout << " .. The specified volume supports Serial Number Update (USN) logs." << endl;
				if (lpFileSystemFlags & FILE_UNICODE_ON_DISK)
					cout << " .. The specified volume supports Unicode in file names as they appear on disk." << endl;
				if (lpFileSystemFlags & FILE_VOLUME_IS_COMPRESSED)
					cout << " .. The specified volume is a compressed volume." << endl;
				if (lpFileSystemFlags & FILE_VOLUME_QUOTAS)
					cout << " .. The specified volume supports disk quotas." << endl;

                cout << "--- ... --- ... --- ..." << endl;

				cout << "file system name: " << lpFileSystemNamebuffer1 << endl;
			}

			if (GetDiskFreeSpaceW(name, &sectorsPerCluster, &bytesPerSector,
				&numberOfFreeClusters, &totalNumberOfClusters)) {
				cout << endl << ".. number of sectors in the cluster:" << sectorsPerCluster << endl;
				cout <<         ".. number of bytes in a sector:" << bytesPerSector << endl;
				cout <<         ".. number of free clusters:" << numberOfFreeClusters << endl;
				cout <<         ".. total number of clusters:" << totalNumberOfClusters << endl;
			}

			system("pause");
			break;

		case 3:

		    // Решение Тртьего пункта -Создание и удаление заданных каталогов, используя функцию CreateDirectoryW() и RemoveDirectoryW()

			cout << "1 - create directory" << endl;
			cout << "2 - remove directory" << endl;

			cin >> menu;
			system("cls");

			switch (menu) {
			case 1:
				cout << "enter the name of directory (example: c:\\directory_name): ";
				cin >> name_of_disk;

				w_string = wstring(name_of_disk.begin(), name_of_disk.end());
				direct_name = w_string.c_str();
				if (!CreateDirectoryW(direct_name, nullptr)) {
					cout << "can't create the directory!" << endl;
				}
				else {
					cout << "directory is succesfully created!" << endl;
				}

				system("pause");
				break;

			case 2:

				cout << "enter the name of directory for remove (example: c:\\directory_name): ";
				cin >> name_of_disk;

				//RemoveDirectory

				w_string = wstring(name_of_disk.begin(), name_of_disk.end());
				direct_name = w_string.c_str();

				if (!RemoveDirectoryW(direct_name)) {
					cout << "can't find entered directory" << endl;
				}
				else {
					cout << "directory is succesfully removed!" << endl;
				}

				system("pause");
				break;

			}

			break;

		case 4:

            // Решение Четвёртого пункта - Создание в указанном каталоге, используя функцию CreateFileW()


			cout << "enter the name of file for created (example: c:\\directory_name\\any.txt):";
			cin >> name_of_disk;

			w_string = wstring(name_of_disk.begin(), name_of_disk.end());
			file_name = w_string.c_str();
			if (CreateFileW(file_name, GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr) == INVALID_HANDLE_VALUE) {
				cout << "can't create a file. error: " << GetLastError();
			}
			else {
				cout << "file is succesfully created!" << endl;
			}

			system("pause");
			break;

		case 5:

		    // Решение Пятого пункта - Копирование и перемещение файлов между каталогамие, используя функции CopyFileW(), MoveFileW(), MoveFileExW()

            cout << "1 - copy file" << endl;
			cout << "2 - move file" << endl;
			cout << "3 - move file (validate)" << endl;

			cin >> menu;
			system("cls");

			switch (menu) {
			case 1:

				cout << "enter the originally file name for copy (example: c:\\directory_name\\any.txt): ";
				cin >> name_of_disk;
				cout << "enter the copying file name (example: c:\\directory_name\\any2.txt): ";
				cin >> name_of_disk2;

				//CopyFile

				w_string = wstring(name_of_disk.begin(), name_of_disk.end());
				firstFile = w_string.c_str();
				w_string_2 = wstring(name_of_disk2.begin(), name_of_disk2.end());
				secondFile = w_string_2.c_str();

				if (CopyFileW(firstFile, secondFile, false))
				{
					cout << "file is succesfully copy!" << endl;
				}
				else {
					cout << "can't copy a file. error: " << GetLastError() << endl;
				}

				system("pause");
				break;

			case 2:

				cout << "enter the file name for move (example: c:\\directory_name\\any.txt): ";
				cin >> name_of_disk;
				cout << "enter the adress (example: c:\\directory_name\\any.txt): ";
				cin >> name_of_disk2;

				w_string = wstring(name_of_disk.begin(), name_of_disk.end());
				firstFile = w_string.c_str();
				w_string_2 = wstring(name_of_disk2.begin(), name_of_disk2.end());
				secondFile = w_string_2.c_str();

				if (MoveFileW(firstFile, secondFile)) {
					cout << "file is succesfully moved!" << endl;
				}
				else {
					cout << "can't move a file. error: " << GetLastError() << endl;
				}

				system("pause");
				break;

			case 3:

				cout << "enter the file name (example: c:\\folder\\any.pdf): ";
				cin >> name_of_disk;
				cout << "enter the adress (example: c:\\folder\\any.pdf): ";
				cin >> name_of_disk2;

				w_string = wstring(name_of_disk.begin(), name_of_disk.end());
				firstFile = w_string.c_str();
				w_string_2 = wstring(name_of_disk2.begin(), name_of_disk2.end());
				secondFile = w_string_2.c_str();

				moveFileEx_temp = 0;

				if (MoveFileExW(firstFile, secondFile, NULL)) {
					cout << "file is succesfully moved!" << endl;
				}
				else if (GetLastError() == 183) {
					cout << "file is already exists. replace (1 (yes)/0 (no))?" << endl;

					cin >> moveFileEx_temp;
					system("cls");

					if (moveFileEx_temp)
					{
						if (MoveFileExW(firstFile, secondFile, MOVEFILE_REPLACE_EXISTING))
						{
							cout << "file is succesfully moved!" << endl;
						}
						else {
							cout << "can't move a file. error: " << GetLastError() << endl;
						}
					}
				}
				else {
					cout << "can't move a file. error: " << GetLastError() << endl;
				}

				system("pause");
				break;
			}

			system("pause");
			break;

		case 6:

		    // Решение Шестого пункта - Анализ и изменение атрибутов файлов, используя функции GetFileAttributesW(), MoveFileW(), MoveFileExW()

			out_attr();
			cin >> menu;
			system("cls");
			switch (menu) {
			case 1:

				cout << "enter the file name (example: c:\\folder\\text.pdf): ";
				cin >> name_of_disk;

				w_string = wstring(name_of_disk.begin(), name_of_disk.end());
				file_name = w_string.c_str();
				attribute = GetFileAttributesW(file_name);

				if (attribute == 0) {
					cout << "can't get attributes of file." << endl;
				}
				else if (attribute == -1) {
					cout << "can't find the file." << endl;
				}
				else {
					cout << "Attributes: (" << attribute << "):" << endl;
					if (FILE_ATTRIBUTE_ARCHIVE & attribute) cout <<    ".. Archive file" << endl;
					if (FILE_ATTRIBUTE_COMPRESSED & attribute) cout << ".. Compressed file" << endl;
					if (FILE_ATTRIBUTE_DIRECTORY & attribute) cout <<  ".. Catalog" << endl;
					if (FILE_ATTRIBUTE_HIDDEN & attribute) cout <<     ".. Hidden file or directory" << endl;
					if (FILE_ATTRIBUTE_NORMAL & attribute) cout <<     ".. The file has no attributes" << endl;
					if (FILE_ATTRIBUTE_READONLY & attribute) cout <<   ".. The file is read-only" << endl;
				}

				system("pause");
				break;

			case 2:

				cout << "enter the file name (example: c:\\folder\\any.pdf): ";
				cin >> name_of_disk;

				//SetFileAttributes

				w_string = wstring(name_of_disk.begin(), name_of_disk.end());
				file_name = w_string.c_str();

				cout << "Change the file attribute:" << endl;
				cout << "1 - Archive file" << endl;
				cout << "2 - Hidden File" << endl;
				cout << "3 - File without attributes" << endl;
				cout << "4 - The file is read-only" << endl;
				cout << "5 - System file" << endl;

				cin >> setAttr;
				system("cls");

				switch (setAttr) {
				case 1:
					if (SetFileAttributesW(file_name,
						FILE_ATTRIBUTE_ARCHIVE)) {
						cout << "the file attribute has been successfully changed" << endl;
					}
					else {
						cout << "error. the file attribute could not be changed: " << GetLastError() << endl;
					}
					break;
				case 2:
					if (SetFileAttributesW(file_name,
						FILE_ATTRIBUTE_HIDDEN)) {
						cout << "the file attribute has been successfully changed" << endl;
					}
					else {
						cout << "error. the file attribute could not be changed: " << GetLastError() << endl;
					}
					break;
				case 3:
					if (SetFileAttributesW(file_name,
						FILE_ATTRIBUTE_NORMAL)) {
						cout << "the file attribute has been successfully changed" << endl;
					}
					else {
						cout << "error. the file attribute could not be changed: " << GetLastError() << endl;
					}
					break;
				case 4:
					if (SetFileAttributesW(file_name,
						FILE_ATTRIBUTE_READONLY)) {
						cout << "the file attribute has been successfully changed" << endl;
					}
					else {
						cout << "error. the file attribute could not be changed: " << GetLastError() << endl;
					}
					break;
				case 5:
					if (SetFileAttributesW(file_name,
						FILE_ATTRIBUTE_SYSTEM)) {
						cout << "the file attribute has been successfully changed" << endl;
					}
					else {
						cout << "error. the file attribute could not be changed: " << GetLastError() << endl;
					}
					break;
				}

				system("pause");
				break;
			case 3:

				long handleInfo, FileAttributes;
				BY_HANDLE_FILE_INFORMATION infoFile;
				HANDLE handleFile;

				cout << "enter the file name (example: c:\\folder\\any.pdf): ";
				cin >> name_of_disk;

				handleFile = CreateFileA((LPCSTR)name_of_disk.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
				handleInfo = GetFileInformationByHandle(handleFile, &infoFile);
				if (!handleInfo)
					cout << "error: " << GetLastError() << endl;
				else {
					FileAttributes = infoFile.dwFileAttributes;
					if (FileAttributes == -1)
						cout << " can't find " << name_of_disk << " file" << endl;
					else {

						cout << "file Information by descriptor:" << endl;

						if (FileAttributes & FILE_ATTRIBUTE_ARCHIVE)
							cout << " file " << name_of_disk << " - Archive" << endl;
						if (FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							cout << "file " << name_of_disk << " - directory" << endl;
						if (FileAttributes & FILE_ATTRIBUTE_READONLY)
							cout << "file " << name_of_disk << " - read-only" << endl;
						if (FileAttributes & FILE_ATTRIBUTE_SYSTEM)
							cout << "file " << name_of_disk << " - System" << endl;
						if (FileAttributes & FILE_ATTRIBUTE_COMPRESSED)
							cout << "file " << name_of_disk << " - compressed" << endl;
						if (FileAttributes & FILE_ATTRIBUTE_HIDDEN)
							cout << "file " << name_of_disk << " - hidden" << endl;
						if (FileAttributes & FILE_ATTRIBUTE_NORMAL)
							cout << "the file does not have such attributes";
						if (FileAttributes == 0)
							cout << "error: " << GetLastError() << endl;
					}

				}
				handleInfo = CloseHandle(handleFile);
				cout << endl;

				system("pause");
				break;

			case 4:
				cout << "enter the file name (example: c:\\folder\\any.pdf): ";
				cin >> name_of_disk;

				fileLPC = name_of_disk.c_str();
				fileT = CreateFileA(fileLPC, GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
										CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				if (GetFileTime(fileT, &timeCreate, &timeLastAccess, &timeLastWrite))
				{
					FileTimeToSystemTime(&timeCreate, &time);
					cout << "date and time of file creation: " <<
						time.wYear << "/" << time.wMonth << "/" << time.wDay << "   " << time.wHour + 3 << ":" <<
						time.wMinute << ":" << time.wSecond << endl;
					FileTimeToSystemTime(&timeLastAccess, &time);
					cout << "date and time of the last access to the file: " <<
						time.wYear << "/" << time.wMonth << "/" << time.wDay << "   " << time.wHour + 3 << ":" <<
						time.wMinute << ":" << time.wSecond << endl;
					FileTimeToSystemTime(&timeLastWrite, &time);
					cout << "date and time of the last modification of the file:" <<
						time.wYear << "/" << time.wMonth << "/" << time.wDay << "   " << time.wHour + 3 << ":" <<
						time.wMinute << ":" <<time.wSecond << endl;
				}
				else {
					cout << "error: " << GetLastError() << endl;
				}

				system("pause");
				break;
			case 5:
				cout << "enter the file name (example: c:\\folder\\any.pdf): ";
				cin >> name_of_disk;

				//SetFileTime

				fileLPC = name_of_disk.c_str();
				fileT = CreateFileA(fileLPC, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
										nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				GetSystemTime(&systemTime);
				SystemTimeToFileTime(&systemTime, &fileTime);
				if (SetFileTime(fileT, &fileTime, &fileTime, &fileTime))
				{
					cout << "time attributes have been replaced successfully!" << endl;
				}
				else {
					cout << "error: " << GetLastError() << endl;
				}

				system("pause");
				break;

			}

			break;

		default:

			break;

		}

	} while (menu != 0);
	return 0;
}
