#include <stdio.h>
#include "windows.h"
#include "iostream"
#include <locale.h>

using namespace std;

void PrintErrorInfo();
void PrintInfo();
void NoArguments();
void PrintMemoryInfo();
void PrintHardwareInfo();
bool MemoryError();
void PrintError();


int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "rus");

	if (argc != 2)
	{
		NoArguments();
	}
	else
	{
		if (strcmp(argv[1], "-e") == 0)
		{
			PrintErrorInfo();
		}
		else if (strcmp(argv[1], "-s") == 0)
		{
			PrintHardwareInfo();
		}
		else
		{
			NoArguments();
		}
	}
	return 0;
}

void PrintErrorInfo()
{
	if (MemoryError())
	{
		PrintError();
	}
	else
	{
		printf("***Memory error!***");
	}
}

void PrintInfo()
{
	PrintMemoryInfo();
	PrintHardwareInfo();
}

bool MemoryError()
{
	DWORD memory = 0;
	MEMORYSTATUS memoryInfo;
	GlobalMemoryStatus(&memoryInfo);
	HLOCAL pAllocMem = LocalAlloc(LMEM_FIXED, memoryInfo.dwTotalVirtual + 5000000000); // total virtual memory + 5 GB
	if (pAllocMem == NULL)
	{
		return true;
	}
	else
	{
		LocalFree(pAllocMem);
		return false;
	}
}

void PrintError()
{
	LPVOID lpvMessage;
	DWORD error = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL, error,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPWSTR)& lpvMessage, 0, NULL);
	printf("\n%s\n", lpvMessage);
	LocalFree(lpvMessage);
	return;
}

void NoArguments()
{
	printf("No arguments. Please use one of these:\n");
	printf("'-e' - print error info\n");
	printf("'-s' - print system information\n");
	exit(EXIT_FAILURE);
}

void PrintMemoryInfo()
{
	MEMORYSTATUS memoryInfo;
	GlobalMemoryStatus(&memoryInfo);
	cout << "Physical and virtual memory" << endl;
	cout << "Physical memory that is in use now:"
		<< memoryInfo.dwMemoryLoad << endl;
	cout << "Physical memory, in bytes:"
		<< memoryInfo.dwTotalPhys << endl;
	cout << "Physical memory that is available, in bytes:"
		<< memoryInfo.dwAvailPhys << endl;
	cout << "Committed memory limit, in bytes:"
		<< memoryInfo.dwTotalPageFile << endl;
	cout << "How much memory the current process can commit, in bytes:"
		<< memoryInfo.dwAvailPageFile << endl;
	cout << "Virtual memory, in bytes:"
		<< memoryInfo.dwTotalVirtual << endl;
	cout << "Virtual memory that is available, in bytes:"
		<< memoryInfo.dwAvailVirtual << endl;
}

void PrintHardwareInfo()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	printf("  Hardware information\n");
	printf("  Processor architecture: %u\n", systemInfo.wProcessorArchitecture);
	printf("  Processor type: %u\n", systemInfo.dwProcessorType);
	printf("  Number of processors: %u\n", systemInfo.dwNumberOfProcessors);
	printf("  Active processor mask: %u\n", systemInfo.dwActiveProcessorMask);
	printf("  Page size: %u\n", systemInfo.dwPageSize);
	printf("  Minimum application address: %p\n", systemInfo.lpMinimumApplicationAddress);
	printf("  Maximum application address: %p\n", systemInfo.lpMaximumApplicationAddress);
	printf("  The granularity for the starting address at which virtual memory can be allocated: %u\n", systemInfo.dwAllocationGranularity);
}