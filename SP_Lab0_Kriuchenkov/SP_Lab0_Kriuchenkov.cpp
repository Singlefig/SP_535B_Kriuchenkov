// SP_Lab0_Kriuchenkov.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "limits.h"
#include <iostream>

using namespace std;

int main()
{
	cout << "Data type: Char. Size in bytes              |" << sizeof(char) <<"|Min value|"<< CHAR_MIN <<"       |Max value|"<< CHAR_MAX<<"|\n";
	cout << "Data type: Signed Char. Size in bytes       |" << sizeof(signed char) << "|Min value|" << SCHAR_MIN << "       |Max value|" << SCHAR_MAX << "|\n";
	cout << "Data type: Unsigned Char. Size in bytes     |" << sizeof(unsigned char) << "|Min value|" << 0 << "          |Max value|" << UCHAR_MAX << "|\n";
	cout << "Data type: Short. Size in bytes             |" << sizeof(short) << "|Min value|" << SHRT_MIN << "     |Max value|" << SHRT_MAX << "|\n";
	cout << "Data type: Unsigned Short Int. Size in bytes|" << sizeof(unsigned short) << "|Min value|" << 0 << "          |Max value|" << USHRT_MAX << "|\n";
	cout << "Data type: Int. Size in bytes               |" << sizeof(int) << "|Min value|" << INT_MIN << "|Max value|" << INT_MAX << "|\n";
	cout << "Data type: Unsigned Int. Size in bytes      |" << sizeof(unsigned int) << "|Min value|" << 0 << "          |Max value|" << UINT_MAX << "|\n";
	cout << "Data type: Long. Size in bytes              |" << sizeof(long) << "|Min value|" << LONG_MIN << "|Max value|" << LONG_MAX << "|\n";
	cout << "Data type: Unsigned Long. Size in bytes     |" << sizeof(unsigned long) << "|Min value|" << 0 << "          |Max value|" << ULONG_MAX << "|\n";
}

