#pragma once

#include <atlstr.h>	// for declaration of CString and LPCTSTR

#ifndef _VSTRING
#define _VSTRING

#include <vector>
using namespace std;

typedef vector<CString> VString;
typedef vector<CString>::const_iterator c_Vit;
typedef vector<CString>::iterator Vit;

#endif

class ProfileManager
{
public:
	ProfileManager(LPCTSTR szProfile = NULL);
	~ProfileManager(void);

private:
	static const int _MaxString = 32767;
	// for line operations
	static UINT m_iCurLine;
	CString m_szLastSection;

	CString m_szProfilePath;

public:
	void	SetProfile(const LPCTSTR szProfile);
	CString GetProfile();

	void	DeleteSection(LPCTSTR szSection);
	void	DeleteKeyName(LPCTSTR szSection, LPCTSTR szKeyName);

	VString ReadSection(LPCTSTR szSection, DWORD nSize = 32767);
	void    WriteSection(LPCTSTR szSection, VString& vTemp);

	// test function, 
	BOOL	ReadLine(LPCTSTR szSection, CString& szLine);
	void    WriteLine(LPCTSTR szSection, CString szLine);
	// test function

	int		ReadInt(LPCTSTR szSection, LPCTSTR szKeyName, int iValue = 0);
	void	WriteInt(LPCTSTR szSection, LPCTSTR szKeyName, int iValue);

	double  ReadDouble(LPCTSTR szSection, LPCTSTR szKeyName, double dValue = 0.0);
	void    WriteDouble(LPCTSTR szSection, LPCTSTR szKeyName, double dValue);

	CString ReadString(LPCTSTR szSection, LPCTSTR szKeyName, LPCTSTR szValue = _T(""));
	void    WriteString(LPCTSTR szSection, LPCTSTR szKeyName, LPCTSTR szValue);

	void    ClearProfile();


};

