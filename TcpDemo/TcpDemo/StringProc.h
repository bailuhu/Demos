#pragma once

//ansiתUnicode
int AnsiToUnicode(char *str, wchar_t* wStr, int size);
wchar_t* AnsiToUnicode(char *str);

//Unicodeתansi
int UnicodeToAnsi(wchar_t *wStr, char* str, int size);
char* UnicodeToAnsi(wchar_t *wStr);

char* UnicodeToAnsi(CString str);
//UnicodeתUTF8
char* UnicodeToUtf8(wchar_t *wStr);
wchar_t* Utf8ToUnicode(char *str);//UTF8תUnicode