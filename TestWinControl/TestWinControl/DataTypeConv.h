#pragma once

#include <atlstr.h>
#include <atlconv.h>
#include <string>
using namespace std;

// CString Char interconversion
char* CStringToChar(CString src);
CString CharToCString(char* src);

// TChar Char interconversion
void TCharToChar(const TCHAR* src, char* dst);
void CharToTChar(const char* src, TCHAR* dst);

// string CString interconversion
CString StringToCString(string src);
string CStringToString(CString src);

// string char interconversion
char*  StringToChar(string& src);
string CharToString(char* src);

// CString decimal/hexadecimal interconversion
int CStringToDec(CString src);
int CStringToHex(CString src);
CString DecToCString(int src);
CString HexToCString(int src);

// Char decimal/hexadecimal interconversion
int CharToDec(char* src);
int CharToHex(char* src);
char* DecToChar(int src);	// extended function: need to delete[] char
char* HexToChar(int src);	// extended function: need to delete[] char

int TCharToDec(TCHAR* src);
int TCharToHex(TCHAR* src);

// mac address convert to longlong type number
long long MacToNum(char* src);	// ignore the split char, both "d4-3e-7e-bb-c9-1b" or "d4:3e:7e-bb-c9-1b" are ok;
CString NumToCStringMac(long long src, char token = '-');
CString NumToCStringIp(long long src, char token = '.');
// ip address convert to longlong type number
long long IpToNum(char* src);

// AnsiתUnicode
int AnsiToUnicode(char *szTemp, wchar_t* wStr, int size);
wchar_t* AnsiToUnicode(char *szTemp);

// UnicodeתAnsi
int UnicodeToAnsi(wchar_t *wStr, char* szTemp, int size);
char* UnicodeToAnsi(wchar_t *wStr);