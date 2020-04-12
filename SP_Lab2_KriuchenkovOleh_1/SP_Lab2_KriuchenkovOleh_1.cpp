#define _CRT_SECURE_NO_WARNINGS

#include "shlwapi.h"
#include "windows.h"
#include "TCHAR.h"
#include <iostream>
#include <stdio.h>

#define PATH_MAX (MAX_PATH + 1) 

void PrintMenu();
bool ChangeDir(TCHAR* sCurrDirectory, BOOL isStdinContainNewlineChar);
bool PrintDir(TCHAR sCurrDirectory[]);
void GetPath(TCHAR sBuffNewPath[], BOOL isUsedBeforeInputChar);
bool CreateDirOwn(BOOL isStdinContainNewlineChar);
bool CopyFileOwn(BOOL isStdinContainNewlineChar);
bool DeleteEmptyDirOrFile(BOOL isStdinContainNewlineChar);
bool PrintInfoAboutFile(BOOL isStdinContainNewlineChar);
void PrintError();


using namespace std;

int main()
{
	setlocale(LC_ALL, ".OCP");
	TCHAR sCurPath[PATH_MAX]{ '\0' };

	GetCurrentDirectory(PATH_MAX, sCurPath);
	char menuChoise;
	while (1)
	{
		PrintMenu();
		wcout << "Current path:" << sCurPath << endl;
		wcout << "Input your choise:";
		cin >> menuChoise;

		switch (menuChoise)
		{
		case '1':
		{
			if (!ChangeDir(sCurPath, TRUE))
			{
				PrintError();
			}
			else
			{
				wcout << "Changed successfully!";
			}
		} break;
		case '2':
		{
			if (!PrintDir(sCurPath))
			{
				PrintError();
			}
			else
			{
				wcout << "Printed successfully!";
			}
		} break;
		case '3':
		{
			if (!CopyFileOwn(TRUE))
			{
				PrintError();
			}
			else
			{
				wcout << "Copied successfully!";
			}
		} break;
		case '4':
		{
			if (!CreateDirOwn(TRUE))
			{
				PrintError();
			}
			else
			{
				wcout << "Created successfully!";
			}
		} break;
		case '5':
		{
			if (!DeleteEmptyDirOrFile(TRUE))
			{
				PrintError();
			}
			else
			{
				wcout << "Deleted successfully!";
			}
		} break;
		case '6':
		{
			if (!PrintInfoAboutFile(TRUE))
			{
				PrintError();
			}
			else
			{
				wcout << "Printed successfully!";
			}
		} break;
		case '7':
		{
			return 0;
		} break;
		default:
		{
			cout << "Error! Incorrect menu choice\n";
		} break;
		}
	}
	return 1;
}

bool ChangeDir(TCHAR* sCurrDirectory, BOOL isStdinContainNewlineChar)
{
	TCHAR sNewPath[PATH_MAX];
	cout << "Input new path:";
	GetPath(sNewPath, isStdinContainNewlineChar);
	if (PathIsDirectory(sNewPath) == FALSE)
	{
		wcout << sNewPath << endl;
		return false;
	}
	wcscpy(sCurrDirectory, sNewPath);
	return true;
}

bool PrintDir(TCHAR sCurrDirectory[])
{
	TCHAR s[PATH_MAX];
	HANDLE hFiles;
	WIN32_FIND_DATAW fd;
	_sntprintf(s, PATH_MAX, L"%s\\*\0", sCurrDirectory);
	hFiles = FindFirstFile(s, &fd);
	if (hFiles != INVALID_HANDLE_VALUE)
	{
		wcout << "Directory files:\n";
		do
		{
			if ((wcscmp(L".", fd.cFileName) == 0) || (wcscmp(L"..", fd.cFileName) == 0))
				continue;
			wcout << " - " << fd.cFileName << endl;
		} while (FindNextFile(hFiles, &fd));
		FindClose(hFiles);
		return true;
	}
	return false;
}

bool CreateDirOwn(BOOL isStdinContainNewlineChar)
{
	TCHAR sNewPath[PATH_MAX];
	cout << "Input new directory name:";
	GetPath(sNewPath, isStdinContainNewlineChar);
	if (!CreateDirectory(sNewPath, NULL))
	{
		return false;
	}
	return true;
}

bool CopyFileOwn(BOOL isStdinContainNewlineChar)
{
	TCHAR sExistPath[PATH_MAX];
	TCHAR sNewPath[PATH_MAX];
	wcout << "Input path to existing file:\n";
	GetPath(sExistPath, isStdinContainNewlineChar);
	if (!PathFileExists(sExistPath))
	{
		cout << "File don`t exist\n";
		return false;
	}
	wcout << "Input new file path:\n";
	GetPath(sNewPath, FALSE);
	if (!CopyFile(sExistPath, sNewPath, TRUE))
	{
		//ShowError();
		return false;
	}
	return true;
}

bool DeleteEmptyDirOrFile(BOOL isStdinContainNewlineChar)
{
	TCHAR sPathDeleting[PATH_MAX];
	wcout << "Input full path to empty directory or file for deleting:\n";
	GetPath(sPathDeleting, isStdinContainNewlineChar);
	if (!RemoveDirectory(sPathDeleting))
	{
		if (!DeleteFile(sPathDeleting))
		{
			return false;
		}
	}
	return true;
}

bool PrintInfoAboutFile(BOOL isStdinContainNewlineChar)
{
	WIN32_FIND_DATA fd = { 0 };
	SYSTEMTIME st = { 0 };
	TCHAR sPathDeleting[PATH_MAX];
	wcout << "Input path to file for detailed info:\n";
	GetPath(sPathDeleting, isStdinContainNewlineChar);
	if (!FindFirstFile(sPathDeleting, &fd))
	{
		return false;
	}
	wcout << "\t File information:\n" << fd.cFileName << endl;
	wcout << "Filename (alternative name):" << fd.cAlternateFileName << endl;
	wcout << "File size:" << ((fd.nFileSizeHigh * MAXDWORD) + fd.nFileSizeLow) / 1024 << " KB" << endl;
	if (FileTimeToSystemTime(&fd.ftCreationTime, &st))
		wprintf_s(L"Time of create: %d:%d:%d, %d:%d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);
	if (FileTimeToSystemTime(&fd.ftLastAccessTime, &st))
		wprintf_s(L"Time of last access: %d:%d:%d, %d:%d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);
	if (FileTimeToSystemTime(&fd.ftLastWriteTime, &st))
		wprintf_s(L"Time of last write: %d:%d:%d, %d:%d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);
	wcout << "File attributes:\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		wcout << " - Archive\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
		wcout << " - Compressed\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
		wcout << " - Hidden\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		wcout << " - Normal\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)
		wcout << " - Unenable file data\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		wcout << " - Readonly\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		wcout << " - System\n";
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
		wcout << " - Temporary\n";
	return true;
}

void PrintMenu()
{
	cout << "\t\n*** File manager ***\n";
	cout << "1 - Change directory\n";
	cout << "2 - Print directory \n";
	cout << "3 - Copy file\n";
	cout << "4 - Create directory\n";
	cout << "5 - Delete file or empty directory\n";
	cout << "6 - Print detailed information about file\n";
	cout << "7 - Exit\n";
}

void GetPath(TCHAR sBuffNewPath[], BOOL isUsedBeforeInputChar)
{
	memset(sBuffNewPath, '\0', sizeof(sBuffNewPath));
	if (isUsedBeforeInputChar)
		getchar(); // skip first \n (from prev line)
	fgetws(sBuffNewPath, PATH_MAX, stdin);
	sBuffNewPath[(wcschr(sBuffNewPath, '\n') - sBuffNewPath)] = '\0'; // change last \n on \0
}

void PrintError()
{
	LPVOID lpMsgBuf;
	DWORD dwLastError = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwLastError,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR)& lpMsgBuf, 0, NULL);
	wprintf(L"\n%s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
	return;
}