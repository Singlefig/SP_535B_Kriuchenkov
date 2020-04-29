#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <ctime>

using namespace std;

#define CRIT_MODE 0
#define NOT_CRIT_MODE 1

#define countOfThreads 3
#define countOfNumbers 10101110 // 10 101 110

int arr[countOfNumbers];
CRITICAL_SECTION criticalSection;

void getSumNums()
{
	int sum = 0;
	for (int i = 0; i < countOfNumbers; i++)
	{
		sum += arr[i];
	}
	cout << "Sum:" << sum << endl;
}

void getAverageNums()
{
	double avg = 0;
	for (int i = 0; i < countOfNumbers; i++)
	{
		avg += arr[i];
	}
	cout << "\nAverage: " << avg / countOfNumbers << endl;
}

void getMaxNum()
{
	int max = 0;
	for (int i = 0; i < countOfNumbers; i++)
	{
		if (max < arr[i])
		{
			max = arr[i];
		}
	}
	cout << "Max: " << max << endl;
}

void generateRandom()
{
	for (int i = 0; i < countOfNumbers; i++)
	{
		arr[i] = rand() % 100;
	}
}

DWORD WINAPI findSumNums(LPVOID mode)
{
	if (NOT_CRIT_MODE)
	{
		getSumNums();
	}
	else {
		if (!InitializeCriticalSectionAndSpinCount(&criticalSection, 0x00000400))
		{
			return 0;
		}
		EnterCriticalSection(&criticalSection);
		getSumNums();
		LeaveCriticalSection(&criticalSection);
	}

	return 0;
}

DWORD WINAPI findAverageNums(LPVOID mode)
{
	if (NOT_CRIT_MODE)
	{
		getAverageNums();
	}
	else {
		if (!InitializeCriticalSectionAndSpinCount(&criticalSection, 0x00000400))
		{
			return 0;
		}
		EnterCriticalSection(&criticalSection);
		getAverageNums();
		LeaveCriticalSection(&criticalSection);
	}

	return 0;
}

DWORD WINAPI findMaxNums(LPVOID mode)
{

	if (NOT_CRIT_MODE)
	{
		getMaxNum();
	}
	else {
		if (!InitializeCriticalSectionAndSpinCount(&criticalSection, 0x00000400))
		{
			return 0;
		}
		EnterCriticalSection(&criticalSection);
		getMaxNum();
		LeaveCriticalSection(&criticalSection);
	}

	return 0;
}

void DeleteCritSec()
{
	DeleteCriticalSection(&criticalSection);
}

int main()
{
	HANDLE hndlThread[countOfThreads];
	DWORD dwThreadId;
	clock_t startClock, finishClock;

	generateRandom();

	cout << "First mode use critical section" << endl;
	startClock = clock();
	hndlThread[0] = CreateThread(NULL, 0, findSumNums, (LPVOID)CRIT_MODE, 0, &dwThreadId);
	hndlThread[1] = CreateThread(NULL, 0, findAverageNums, (LPVOID)CRIT_MODE, 0, &dwThreadId);
	hndlThread[2] = CreateThread(NULL, 0, findMaxNums, (LPVOID)CRIT_MODE, 0, &dwThreadId);
	WaitForMultipleObjects(countOfThreads, hndlThread, TRUE, INFINITE);

	finishClock = clock();
	double duration = finishClock - startClock;
	cout << "Time: " << duration << endl;

	DeleteCritSec();

	for (int i = 0; i < countOfThreads; i++)
	{
		CloseHandle(hndlThread[i]);
	}

	cout << "Second mode not use critical section" << endl;
	startClock = clock();

	hndlThread[0] = CreateThread(NULL, 0, findSumNums, (LPVOID)NOT_CRIT_MODE, 0, &dwThreadId);
	hndlThread[1] = CreateThread(NULL, 0, findAverageNums, (LPVOID)NOT_CRIT_MODE, 0, &dwThreadId);
	hndlThread[2] = CreateThread(NULL, 0, findMaxNums, (LPVOID)NOT_CRIT_MODE, 0, &dwThreadId);
	WaitForMultipleObjects(countOfThreads, hndlThread, TRUE, INFINITE);

	finishClock = clock();
	duration = finishClock - startClock;
	cout << "Time: " << duration << endl;

	DeleteCritSec();

	for (int i = 0; i < countOfThreads; i++)
	{
		CloseHandle(hndlThread[i]);
	}

	return 0;
}