#include "stdafx.h"
#include "StringProc.h"

wchar_t* AnsiToUnicode(char *str)
{
	int cnt = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0); //包含/0
	wchar_t *wStr;
	wStr = new wchar_t[cnt];

	MultiByteToWideChar (CP_ACP, 0, str, -1, wStr, cnt);
	return wStr;
}

int AnsiToUnicode(char *str, wchar_t* wStr, int size)
{
	int cnt = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0);

	if (cnt > size) return -1;

	MultiByteToWideChar (CP_ACP, 0, str, -1, wStr, size);

	return 0;
}

//Unicode转ansi
int UnicodeToAnsi(wchar_t *wStr, char* str, int size)
{
	int cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);

	if (cnt > size) return -1;

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, str, size, NULL, FALSE);
	return 0;
}

//Unicode转ansi
char* UnicodeToAnsi(wchar_t *wStr)
{
	int Cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);
	char *str;
	str = new char[Cnt];

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, str, Cnt, NULL, FALSE);
	return str;
}


char* UnicodeToAnsi(CString str)
{
	int n = str.GetLength(); 
	int len = WideCharToMultiByte(CP_ACP, 0, str, n, NULL, 0, NULL, NULL);
	char *pChStr = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, n, pChStr, len, NULL, NULL);
	pChStr[len] = '\0';

	return pChStr;
}
//Unicode转UTF8
char* UnicodeToUtf8(wchar_t *wStr)
{
	int Cnt = WideCharToMultiByte(CP_UTF8, NULL, wStr, -1, NULL, 0, NULL, 0);
	char *str;
	str = new char[Cnt];

	WideCharToMultiByte (CP_UTF8, NULL, wStr, -1, str, Cnt, NULL, 0);
	return str;
}

//UTF8转Unicode
wchar_t* Utf8ToUnicode(char *str)
{
	int cnt = MultiByteToWideChar (CP_UTF8, 0, str, -1, NULL, 0); //包含/0
	wchar_t *wStr;
	wStr = new wchar_t[cnt]; 

	MultiByteToWideChar (CP_UTF8, 0, str, -1, wStr, cnt);
	return wStr;
}