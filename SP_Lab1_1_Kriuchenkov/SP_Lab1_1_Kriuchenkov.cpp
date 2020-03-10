#include <stdio.h>
#include "windows.h"
#include <locale.h>
#include <iostream>

using namespace std;



void PrintError()
{
	LPVOID lpvMessage;
	DWORD dwError = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)& lpvMessage, 0, NULL);
	printf("\n%s\n", lpvMessage);
	LocalFree(lpvMessage);
	return;
}

void EncodeToANSIFromUnicode(LPCSTR fileName)
{
	HANDLE sourceFile, destinationFile; 

	CHAR buffer[1024];
	WCHAR * encodedCharsToWChar_t;

	DWORD readedChars, writtenChars;  
	BOOL readFile = true;

	char nameOfDestinationFile[FILENAME_MAX];
	snprintf(nameOfDestinationFile, sizeof(nameOfDestinationFile), "%s%s", "UnicodeEncoded", fileName);
	printf("Результат находится в \n %s\n", nameOfDestinationFile);
	sourceFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (sourceFile == INVALID_HANDLE_VALUE)
	{
		PrintError();
		printf("1 error\n");
		exit(EXIT_FAILURE);
	}
	destinationFile = CreateFileA(nameOfDestinationFile, GENERIC_WRITE, 0,NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (destinationFile == INVALID_HANDLE_VALUE)
	{
		PrintError();
		printf("2 error\n");
		exit(EXIT_FAILURE);
	}
	while (((readFile = ReadFile(sourceFile, &buffer, sizeof(buffer), &readedChars, NULL)) != 0) && (readedChars != 0))
	{
		int allocationSize = MultiByteToWideChar(GetACP(), 0, buffer, readedChars, NULL, 0);
		encodedCharsToWChar_t = (WCHAR *)calloc(allocationSize, sizeof(WCHAR));

		MultiByteToWideChar(GetACP(), 0, buffer, readedChars, encodedCharsToWChar_t, allocationSize);

		if (!WriteFile(destinationFile, encodedCharsToWChar_t, readedChars * 2, &writtenChars, NULL))
		{
			PrintError();
			printf("3 error\n");
			exit(EXIT_FAILURE);
		}
		free(encodedCharsToWChar_t);
	}
	if (readFile == false && readedChars != 0)
	{
		PrintError();
		printf("4 error\n");
		exit(EXIT_FAILURE);
	}

	CloseHandle(sourceFile);
	CloseHandle(destinationFile);
}

void EncodeToUnicodeFromANSI(LPCSTR fileName)
{
	HANDLE sourceFile, destinationFile;

	CHAR buffer[1024];
	WCHAR * bytesEncodedIntoWchar;
	CHAR * WCharsEncodedToCHARS;
	BOOL isSymbolUnknown = false;

	DWORD readedBytes, writtenBytes;  
	BOOL readFile = true;

	char nameDestinationFile[FILENAME_MAX];
	snprintf(nameDestinationFile, sizeof(nameDestinationFile), "%s%s", "ASCIEncoded", fileName);
	printf("Результат находится в \n %s\n", nameDestinationFile);
	sourceFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (sourceFile == INVALID_HANDLE_VALUE)
	{
		PrintError();
		exit(EXIT_FAILURE);
	}
	destinationFile = CreateFileA(nameDestinationFile, GENERIC_WRITE, 0,NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (destinationFile == INVALID_HANDLE_VALUE)
	{
		PrintError();
		exit(EXIT_FAILURE);
	}
	if (!ReadFile(sourceFile, &buffer, 2, &readedBytes, NULL))
	{
		PrintError();
		exit(EXIT_FAILURE);
	}
	while (((readFile = ReadFile(sourceFile, &buffer, sizeof(buffer), &readedBytes, NULL)) != false) && (readedBytes != 0))
	{
		int symbols = MultiByteToWideChar(GetACP(),0,buffer,readedBytes,NULL,0);

		bytesEncodedIntoWchar = (WCHAR *)calloc(symbols, sizeof(WCHAR));

		MultiByteToWideChar(GetACP(), 0, buffer, readedBytes,bytesEncodedIntoWchar, symbols);

		int allocationSize = WideCharToMultiByte(CP_ACP,0,bytesEncodedIntoWchar,symbols,NULL,0,NULL,NULL);

		WCharsEncodedToCHARS = (CHAR *)calloc(allocationSize, sizeof(CHAR));

		WideCharToMultiByte(CP_ACP,0, bytesEncodedIntoWchar,symbols,WCharsEncodedToCHARS,allocationSize,NULL,NULL);
		if (!WriteFile(destinationFile, WCharsEncodedToCHARS, allocationSize, &writtenBytes, NULL))
		{
			PrintError();
			exit(EXIT_FAILURE);
		}
		free(bytesEncodedIntoWchar);
		free(WCharsEncodedToCHARS);
	}
	if (readFile == false && readedBytes != 0)
	{
		PrintError();
		exit(EXIT_FAILURE);
	}

	CloseHandle(sourceFile);
	CloseHandle(destinationFile);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	if (argc != 3)
	{
		fprintf(stderr, "***Ошибка в аргументах для программы!***\n");
		fprintf(stderr, "-a  - ANSI\n");
		fprintf(stderr, "-u  - Unicode\n");
		system("Pause");
		return 0;
	}
	else
	{
		printf("%s", argv[1]);
		if (strcmp(argv[1], "-a") == 0)
		{
			EncodeToANSIFromUnicode(argv[2]);
		}
		else if (strcmp(argv[1], "-u") == 0)
		{
			EncodeToUnicodeFromANSI(argv[2]);
		}
		else
		{
			fprintf(stderr, "***Ошибка ввода!!!***\n");
		}
	}
	fprintf(stdout, "Успех!");
	return 0;
}

