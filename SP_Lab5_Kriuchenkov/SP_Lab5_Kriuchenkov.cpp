#include <stdio.h>
#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
	HANDLE mut = CreateMutex(
		0,
		0,
		TEXT("MutexName")
	);
	DWORD res = WaitForSingleObject(mut, 0);

	if (res != WAIT_OBJECT_0)
	{
		cout << "Application is already running" << endl;

		return 0;
	}
	else
	{
		cout << "It`s running" << endl;

		while (1) {}
	}

	return 0;
}
