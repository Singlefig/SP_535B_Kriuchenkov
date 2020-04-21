#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "windows.h"
#include "iostream"
#include "tchar.h"
#include "processthreadsapi.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define PRINTSUB 'a'
#define PRINTPARAM 'b'
#define FINDSTRING 'c'
#define SAVEFILE 'd'
#define EXIT 'e'

using namespace std;

typedef struct {
	TCHAR    nStKey[MAX_KEY_LENGTH];
	DWORD    dwStName;
	DWORD    dwSecDescr;
	FILETIME fltWriteTime;
	DWORD    countOfSubKeys = 0;
	DWORD    MValue;
	DWORD    MValueData;
	DWORD    largestSubKey;
	DWORD    MClass;
	DWORD    nOfValues;
	TCHAR    nStClass[MAX_PATH] = TEXT("");
	DWORD    sStClass = MAX_PATH;
	
	 
} KEY_INFO, *pKEY_INFO;

void ShowMenu();

bool OpenRegKey(HKEY** hKey, DWORD dwOpenAccess, LPSTR fullPath);

void ReadString(CHAR sBuffNewPath[], DWORD maxBuffSize, BOOL isUsedBeforeInputChar);

bool GetInfo(HKEY key, KEY_INFO* keyInfo);

void ShowListSubkeysOfKey(HKEY key);

void ShowListParamsOfKey(HKEY key);

bool FindString(HKEY hKey, LPCSTR reqStr, LPSTR fullPath);

bool SaveKey(HKEY hKey);

BOOL Privilege(
	HANDLE hToken,
	LPCTSTR lpszPrivilege,
	BOOL bEnablePrivilege
);

int main()
{
	char choise = 0;

	for (;;)
	{
		ShowMenu();
		cin >> choise;

		switch (choise)
		{
		case PRINTSUB:
		{
			HKEY hKey = { 0 };
			PHKEY phKey = &hKey;
			if (OpenRegKey(&phKey, KEY_READ, NULL) == true)
			{
				ShowListSubkeysOfKey(hKey);
			}
		}break;
		case PRINTPARAM:
		{
			HKEY hKey = { 0 };
			PHKEY phKey = &hKey;
			if (OpenRegKey(&phKey, KEY_QUERY_VALUE, NULL) == true)
			{
				ShowListParamsOfKey(hKey);
			}
		} break;
		case FINDSTRING:
		{
			HKEY hKey = { 0 };
			PHKEY phKey = &hKey;
			CHAR fullPath[MAX_PATH];
			if (OpenRegKey(&phKey, KEY_ALL_ACCESS, fullPath) == true)
			{
				CHAR reqString[MAX_PATH] = { '\0' };
				cout << "Input string to search for:";
				ReadString(reqString, MAX_PATH, false);
				FindString(hKey, reqString, fullPath);
			}
		} break;
		case SAVEFILE:
		{
			HANDLE hToken;

			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			{
				cout << "Can't get access rights (SE_BACKUP_NAME)\n Error:" << GetLastError() << endl;
			}
			if (Privilege(hToken, SE_BACKUP_NAME, true))
			{
				HKEY hKey = { 0 };
				PHKEY phKey = &hKey;
				if (OpenRegKey(&phKey, KEY_ALL_ACCESS, NULL) == true)
				{
					SaveKey(hKey);
				}
			}
		} break;
		case EXIT:
		{
			return 0;
		} break;
		default:
			cout << "Wrong choise!, try again\n";
			break;
		}
	}

	return 0;
}

bool FindString(HKEY hKey, LPCSTR reqStr, LPSTR fullPath)
{
	KEY_INFO keyInfo = { 0 };
	DWORD retCode = ERROR_SUCCESS;
	LPSTR newSubkeyPath;
	if (!GetInfo(hKey, &keyInfo))
	{
		return false;
	}
	if (keyInfo.countOfSubKeys)
	{
		for (unsigned int i = 0; i < keyInfo.countOfSubKeys; i++)
		{
			keyInfo.dwStName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey,
				i,
				keyInfo.nStKey,
				&keyInfo.dwStName,
				NULL,
				NULL,
				NULL,
				NULL);
			if (retCode == ERROR_SUCCESS)
			{
				if (_strcmpi((char *)keyInfo.nStKey, reqStr) == 0)
				{
					cout << " * Found in subkey name: " << fullPath << "\\" << keyInfo.nStKey << endl;
				}
				newSubkeyPath = (LPSTR)malloc(MAX_VALUE_NAME * sizeof(TCHAR));
				strcpy(newSubkeyPath, fullPath);
				strcat(newSubkeyPath, "\\");
				strcat(newSubkeyPath, (char *)keyInfo.nStKey);
				HKEY newKey = { 0 };
				if (RegOpenKeyEx(hKey, keyInfo.nStKey, 0, KEY_ALL_ACCESS, &newKey) == ERROR_SUCCESS)
				{
					FindString(newKey, reqStr, newSubkeyPath);
				}
				free(newSubkeyPath);
			}
		}
	}
	if (keyInfo.nOfValues)
	{
		LPSTR lpValue = NULL;
		DWORD dwValue = keyInfo.MValue + 1;

		DWORD dwType = 0;

		LPBYTE lpData = NULL;
		DWORD dwData = 0;

		lpValue = (LPSTR)malloc((keyInfo.MValue + 1) * sizeof(BYTE));

		for (unsigned int i = 0; i < keyInfo.nOfValues; i++)
		{
			retCode = RegEnumValueA(hKey, i, lpValue, &dwValue, NULL, NULL, NULL, &dwData);
			lpData = (LPBYTE)malloc((dwData + 1) * sizeof(BYTE));

			dwValue = keyInfo.MValue + 1;

			retCode = RegEnumValueA(hKey,
				i,
				lpValue,
				&dwValue,
				NULL,
				&dwType,
				lpData,
				&dwData);

			if (retCode == ERROR_SUCCESS)
			{
				if (_strcmpi(lpValue, reqStr) == 0)
				{
					cout << " * Found in value name: " << fullPath << "; " << lpValue << endl;
				}
				if (((dwType & REG_EXPAND_SZ) == REG_EXPAND_SZ) || ((dwType & REG_SZ) == REG_SZ))
				{
					if (_strcmpi((LPSTR)lpData, reqStr) == 0)
					{
						cout << " * Found in data of value " << fullPath << "; " << lpValue << ";\n   data:" << lpData << endl;
					}
				}
			}
		}
	}
	RegCloseKey(hKey);
}

BOOL Privilege(
	HANDLE hToken,
	LPCTSTR lpszPrivilege,
	BOOL bEnablePrivilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,
		lpszPrivilege,
		&luid))
	{
		printf("Privilege Value error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
	{
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tp.Privileges[0].Attributes = 0;
	}

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("Adjust Token Privileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("Your user doesn't have SE_BACKUP_NAME privilege \n");
		return FALSE;
	}
	return TRUE;
}

bool SaveKey(HKEY hKey)
{
	CHAR filePath[MAX_PATH];
	DWORD retCode = ERROR_SUCCESS;
	cout << "Input path to file:\n";
	ReadString(filePath, MAX_PATH, false);

	retCode = RegSaveKey(hKey, (LPCWSTR)filePath, NULL);
	switch (retCode)
	{
	case ERROR_SUCCESS:
	{
		cout << "Key saved in file:\n" << filePath << endl;
		RegCloseKey(hKey);
		return true;
	} break;
	case ERROR_ALREADY_EXISTS:
	{
		cout << "Error! File is already exists!\n Your file path:\n" << filePath << endl;
	} break;
	default:
		cout << "Error! Cant save key into file\n Error:" << retCode << endl;
	}
	RegCloseKey(hKey);
	return false;
}

void ShowListParamsOfKey(HKEY key)
{
	DWORD i, retCode = ERROR_SUCCESS;
	KEY_INFO keyInfo = { 0 };

	DWORD dwType = 0;

	LPBYTE lpData = NULL;
	DWORD dwData = 0;

	LPSTR lpValue = NULL;
	DWORD dwValue = 0;

	GetInfo(key, &keyInfo);

	if (keyInfo.nOfValues)
	{
		cout << "\t Count of values:" << keyInfo.nOfValues << endl;
		lpValue = (LPSTR)malloc((keyInfo.MValue + 1) * sizeof(BYTE));
		dwValue = keyInfo.MValue + 1;

		for (unsigned int i = 0; i < keyInfo.nOfValues; i++)
		{
			retCode = RegEnumValueA(key, i, lpValue, &dwValue, NULL, NULL, NULL, &dwData);
			lpData = (LPBYTE)malloc((dwData + 1) * sizeof(BYTE));

			dwValue = keyInfo.MValue + 1;

			retCode = RegEnumValueA(key,
				i,
				lpValue,
				&dwValue,
				NULL,
				&dwType,
				lpData,
				&dwData);

			if (retCode == ERROR_SUCCESS)
			{
				if (strcmp(lpValue, "") == 0)
				{
					printf("\n(%d) Name: %s\n", i + 1, "Default value");
				}
				else
				{
					printf("\n(%d) Name: %s\n", i + 1, lpValue);
				}

				switch (dwType)
				{
				case REG_BINARY:
				{
					printf("    Type: REG_BINARY\n    Data: binary\n");
				} break;
				case REG_DWORD:
				{
					DWORD data = *(DWORD*)(lpData);
					printf("    Type: REG_DWORD\n    Data: %#x|%u\n", data, data);
				} break;
				case REG_EXPAND_SZ:
				{
					printf("    Type: REG_EXPAND_SZ\n    Data: %s\n", lpData);
				} break;
				case REG_LINK:
				{
					wprintf(L"    Type: REG_LINK\n    Data: %ws\n", lpData);
				} break;
				case REG_SZ:
				{
					printf("    Type: REG_SZ\n    Data: %s\n", lpData);
				} break;
				case REG_NONE:
				{
					printf("    Type: REG_NONE\n    Data: %x\n", *(DWORD*)(lpData));
				} break;
				default:
					printf("    Type: unknown\n    Data: %x\n", *(DWORD*)(lpData));
					break;
				}
			}
			free(lpData);
		}
		free(lpValue);
	}
	RegCloseKey(key);
}

void ShowListSubkeysOfKey(HKEY key)
{
	DWORD i, retCode;
	KEY_INFO keyInfo = { 0 };

	GetInfo(key, &keyInfo);

	if (keyInfo.countOfSubKeys)
	{
		cout << "\t Count of subkeys :" << keyInfo.countOfSubKeys << endl;
		for (unsigned int i = 0; i < keyInfo.countOfSubKeys; i++)
		{
			keyInfo.dwStName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(key,
				i,
				keyInfo.nStKey,
				&keyInfo.dwStName,
				NULL,
				NULL,
				NULL,
				NULL);
			if (retCode == ERROR_SUCCESS)
			{
				printf("(%d) %s\n", i + 1, keyInfo.nStKey);
			}
		}
	}
	RegCloseKey(key);
}

void ReadString(CHAR sBuffNewPath[], DWORD maxBuffSize, BOOL isUsedBeforeInputChar)
{
	memset(sBuffNewPath, '\0', sizeof(sBuffNewPath));
	if (isUsedBeforeInputChar)
	{
		getchar();
	}
	fgets(sBuffNewPath, maxBuffSize, stdin);
	if ((strlen(sBuffNewPath) > 0) && (sBuffNewPath[strlen(sBuffNewPath) - 1] == '\n'))
	{
		sBuffNewPath[strlen(sBuffNewPath) - 1] = '\0';
	}
}

bool GetInfo(HKEY key, KEY_INFO * keyInfo)
{
	DWORD retCode = RegQueryInfoKey(key,
		(*keyInfo).nStClass,
		&(*keyInfo).sStClass,
		NULL,
		&(*keyInfo).countOfSubKeys,
		&(*keyInfo).largestSubKey,
		&(*keyInfo).MClass,
		&(*keyInfo).nOfValues,
		&(*keyInfo).MValue,
		&(*keyInfo).MValueData,
		&(*keyInfo).dwSecDescr,
		&(*keyInfo).fltWriteTime);
	if (retCode == ERROR_SUCCESS) return true;
	else return false;
}

bool OpenRegKey(HKEY** hKey, DWORD dwOpenAccess, LPSTR fullPath)
{
	HKEY predKey;
	if (fullPath != NULL)  memset(fullPath, '\0', sizeof(fullPath));
	int choise = 0;
	cout << "Predefined keys:\n";
	cout << "1 - HKEY_CLASSES_ROOT\n";
	cout << "2 - HKEY_CURRENT_USER\n";
	cout << "3 - HKEY_LOCAL_MACHINE\n";
	cout << "4 - HKEY_USERS\n";
	cout << "5 - HKEY_CURRENT_CONFIG\n";
	cout << "6 - HKEY_PERFORMANCE_DATA\n";
	cout << "Choose predefined key:";
	scanf("%d", &choise);
	switch (choise)
	{
	case 1:
	{
		predKey = HKEY_CLASSES_ROOT;
		if (fullPath != NULL) strcpy(fullPath, "HKEY_CLASSES_ROOT\\");
	} break;
	case 2:
	{
		predKey = HKEY_CURRENT_USER;
		if (fullPath != NULL) strcpy(fullPath, "HKEY_CURRENT_USER\\");
	} break;
	case 3:
	{
		predKey = HKEY_LOCAL_MACHINE;
		if (fullPath != NULL) strcpy(fullPath, "HKEY_LOCAL_MACHINE\\");
	} break;
	case 4:
	{
		predKey = HKEY_USERS;
		if (fullPath != NULL) strcpy(fullPath, "HKEY_USERS\\");
	} break;
	case 5:
	{
		predKey = HKEY_CURRENT_CONFIG;
		if (fullPath != NULL) strcpy(fullPath, "HKEY_CURRENT_CONFIG\\");
	} break;
	case 6:
	{
		predKey = HKEY_PERFORMANCE_DATA;
		if (fullPath != NULL) strcpy(fullPath, "HKEY_PERFORMANCE_DATA\\");
	} break;
	default:
		return false;
	}

	CHAR keyArr[MAX_KEY_LENGTH] = { '\0' };
	LPSTR key = keyArr;
	cout << "Input subkey of the chosen predefined key:\n";
	ReadString(key, MAX_KEY_LENGTH, true);
	if (RegOpenKeyExA(predKey, key, 0, dwOpenAccess, *hKey) == ERROR_SUCCESS)
	{
		if (fullPath != NULL) strcat(fullPath, key);
		return true;
	}
	return false;
}

void ShowMenu()
{
	cout << "***Menu***\n";
	cout << "a - Print a list of subkeys by key name\n";
	cout << "b - Print a list of keys parameters with their value and type\n";
	cout << "c - Searches the registry for a given string in the key names, key values and their types.\nBase key set user\n";
	cout << "d - Save key as a file\n";
	cout << "e - Exit\n";
}