#include "stdafx.h"
#include "DataTypeConv.h"

char* CStringToChar(CString src)
{
	// Method 1
	USES_CONVERSION;
	return T2A(src); // W2A(src); 也可实现转换

	/*// Method 2
 	CString src = _T("CString to char");  
 	int len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);      
 	char* dst = new char[len + 1];      
 	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, len, NULL, NULL );  
 	delete[] dst; 
	*/
}

CString CharToCString(char* src)
{
	// method 1
	return src;

	/*// method 2
	USES_CONVERSION;
	return A2T(src); // A2W(src); 也可实现转换
	*/

	/*// method 3
	return (LPSTR)(LPCTSTR)info;
	*/

	/*// method 4
	int charLen = strlen(src);
	int len = MultiByteToWideChar(CP_ACP, 0, src, charLen, NULL, 0);
	TCHAR* buf = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, src, charLen, buf, len);
	buf[len] = '';	// 添加字符串结尾，注意不是len+1
	CString wideChar;
	wideChar.Append(buf);
	delete[] buf;
	*/
}

int CStringToDec(CString src)
{
	// Generic	SBCS/ANSI	UNICODE
	// _ttoi	atoi		_wtoi
	//	return _ttoi(src);	// method 1
	USES_CONVERSION;
	return strtol(T2A(src), NULL, 10); // method 2
}

int CStringToHex(CString src)
{
	USES_CONVERSION;
	return strtol(T2A(src), NULL, 16);
}

int CharToDec(char* src)
{
	return strtol(src, NULL, 10);
}

int CharToHex(char* src)
{
	return strtol(src, NULL, 16);
}

char* DecToChar(int src)
{
	int bit = 1;
	int iTemp = src;
	while(iTemp > 0)
	{
		iTemp /= 10;
		bit++;
	}
	char* dst = new char[bit];
	_itoa_s(src, dst, bit, 10);
	return dst;
}

char* HexToChar(int src)
{
	int bit = 1;
	int iTemp = src;
	while(iTemp > 0)
	{
		iTemp /= 10;
		bit++;
	}
	char* dst = new char[bit];
	_itoa_s(src, dst, bit, 16);
	return dst;
}

void TCharToChar(const TCHAR* src, char* dst)  
{  
	// 获取字节长度   
	int len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, len, NULL, NULL);   	// 将tchar值赋给_char 
}  

void CharToTChar(const char* src, TCHAR* dst)  
{  
	int len = MultiByteToWideChar(CP_ACP, 0, src, strlen(src) + 1, NULL, 0) ;  
	MultiByteToWideChar(CP_ACP, 0, src, strlen(src) + 1, dst, len) ;  
} 

int TCharToDec(TCHAR* src)
{
	int len = _tcslen(src);
	char* dst = new char[len + 1];
	TCharToChar(src, dst);
	int iDst = CharToDec(dst);
	delete[] dst;
	return iDst;
}

int TCharToHex(TCHAR* src)
{
	int len = _tcslen(src);
	char* dst = new char[len + 1];
	TCharToChar(src, dst);
	int iDst = CharToHex(dst);
	delete[] dst;
	return iDst;
}

CString DecToCString(int src)
{
	CString dst;
	dst.Format(_T("%d"), src);
	return dst;
}

CString HexToCString(int src)
{
	CString dst;
	dst.Format(_T("%x"), src);
	return dst;
}

CString StringToCString(string src)
{
	return src.c_str();
}

string CStringToString(CString src)
{
	USES_CONVERSION;
	return T2A(src.GetBuffer()); // 转化为非unicode
}

char* StringToChar(string& src)
{
	char* dst = const_cast<char*>(src.c_str());
//	dst = const_cast<char*>(src.data());
	return dst;
}

string CharToString(char* src)
{
	return src;
}

long long MacToNum(char* src)
{
	if (src == NULL)  return 0;
	long long total = 0;
	unsigned int cur = 0;
	int i = 5;	// 6 section mac address
	char temp[2] = {0};
	int count = 0;
	while (count++ < 18)	// length of mac
	{
		if (count%3 == 0)
		{
			temp[0] = *(src+count-3);
			temp[1] = *(src+count-2);
			cur = CharToHex(temp);
			total += long long(cur * pow(256.0 ,i--));
			continue;
		}
	}
	return total;
}

CString NumToCStringMac(long long src, char token /*= '-'*/)
{
 	CString dst;
	const int len = 6;
	long long iTemp = src;
	char* temp = new char[1];

	for (int i = 0; i < len; i++)
	{
		CString szTemp = HexToCString(iTemp & 0xff);
		iTemp = iTemp>>8;
		dst = szTemp + token + dst;
	}
	dst = dst.Left(dst.GetLength() - 1);
	return dst;
}

long long IpToNum(char* src)
{
	if (src == NULL)  return 0;
	const TCHAR* split = _T(".");
	long long total = 0;
	unsigned int cur = 0;
	int i = 3;	// 4 section mac address
	char temp[3] = {0};
	int count = 0;
	while (count++ < 16)	// length of ip
	{
		if (count%4 == 0)
		{
			temp[0] = *(src+count-4);
			temp[1] = *(src+count-3);
			temp[2] = *(src+count-2);
			cur = CharToDec(temp);
			total += long long (cur * pow(256.0 ,i--));
			continue;
		}
	}
	return total;
}

CString NumToCStringIp(long long src, char token /*= '.'*/)
{
	int i = 3;
	unsigned int cur = 0;
	CString dst;
	CString temp;
	while(i >= 0)
	{
		cur = unsigned int(src/pow(256.0, i--));
		src -= unsigned int(cur*pow(256.0, i+1));
		temp.Format(_T("%03d"), cur);
		dst += temp + token;
	};
	dst = dst.Left(dst.GetLength()-1);
	return dst;
}

wchar_t* AnsiToUnicode(char *szTemp)
{
	int cnt = MultiByteToWideChar (CP_ACP, 0, szTemp, -1, NULL, 0);
	wchar_t *wStr;
	wStr = new wchar_t[cnt];

	MultiByteToWideChar (CP_ACP, 0, szTemp, -1, wStr, cnt);
	return wStr;
}

int AnsiToUnicode(char *szTemp, wchar_t* wStr, int size)
{
	int cnt = MultiByteToWideChar (CP_ACP, 0, szTemp, -1, NULL, 0);

	if (cnt > size) return -1;

	MultiByteToWideChar (CP_ACP, 0, szTemp, -1, wStr, size);

	return 0;
}

// Unicode转ansi
int UnicodeToAnsi(wchar_t *wStr, char* szTemp, int size)
{
	int cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);

	if (cnt > size) return -1;

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, szTemp, size, NULL, FALSE);
	return 0;
}

// Unicode转ansi
char* UnicodeToAnsi(wchar_t *wStr)
{
	int Cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);
	char *szTemp;
	szTemp = new char[Cnt];

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, szTemp, Cnt, NULL, FALSE);
	return szTemp;
}