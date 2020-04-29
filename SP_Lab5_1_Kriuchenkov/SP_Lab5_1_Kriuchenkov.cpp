#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <iostream>
#include <ctime>

using namespace std;

#define countOfThreads 10
#define countInitSemaphore 3
#define maxCountSemaphore 3

HANDLE sem;

void showError() {
	setlocale(LC_ALL, "Russian");
	cout << "Error" << endl;

	LPVOID errorMessage;
	DWORD eCode = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		eCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorMessage, 0, NULL);
	char* errMessage = (char*)errorMessage;
	wprintf(L"ERROR was generated: failed with 0x%x code\n", eCode);
	wprintf(L"ERROR formatted message: %s\n", errorMessage);
}


DWORD WINAPI WorkedByThread(LPVOID lpParam)
{
	DWORD state, bytesToWrite;
	HANDLE file;
	clock_t startClock, finishClock;
	char* string = new char[10];
	bool run = true;
	const int second = 1000;
	state = WaitForSingleObject(sem, 3);

	while (run)
	{
		if (state == WAIT_OBJECT_0)
		{
			startClock = clock();

			Sleep(3 * second);
			cout << "Thread: " << GetCurrentThreadId() << " is in critical section" << endl;

			file = CreateFileA("resultFile.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			finishClock = clock();
			
			double duration = finishClock - startClock;

			SetFilePointer(file, 0, NULL, FILE_END);
			sprintf(string, "%f", duration);

			WriteFile(file, string, sizeof(string), &bytesToWrite, NULL);

			if (file == INVALID_HANDLE_VALUE)
			{
				showError();
			}
			run = false;

			Sleep(3 * second);

			cout << "Thread:" << GetCurrentThreadId() << " is not in critical section anymore" << endl;

			if (!ReleaseSemaphore(sem, 1, NULL))
			{
				showError();
			}
		}
	}
	return 0;
}

int main()
{
	HANDLE threads[countOfThreads];
	DWORD id;

	sem = CreateSemaphore(
		NULL,
		countInitSemaphore,
		maxCountSemaphore,
		NULL
	);

	if (sem == NULL)
	{
		showError();

		return 1;
	}

	for (int i = 0; i < countOfThreads; i++)
	{
		threads[i] = CreateThread(NULL, 0, WorkedByThread, NULL, 0, &id);

		if (threads[i] == NULL)
		{
			showError();

			return 1;
		}
	}

	WaitForMultipleObjects(countOfThreads, threads, TRUE, INFINITE);
	for (int i = 0; i < countOfThreads; i++)
	{
		CloseHandle(threads[i]);
	}
	CloseHandle(sem);

	return 0;
}