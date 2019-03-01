#include "StdAfx.h"
#include "ProfileManager.h"


UINT ProfileManager::m_iCurLine = 0;

ProfileManager::ProfileManager(LPCTSTR szProfile /*= NULL*/)
	:m_szProfilePath(szProfile)
{
}


ProfileManager::~ProfileManager(void)
{
}

void ProfileManager::SetProfile(LPCTSTR szProfile)
{
	m_szProfilePath = szProfile;
}

CString ProfileManager::GetProfile()
{
	return m_szProfilePath;
}

VString ProfileManager::ReadSection(LPCTSTR szSection, DWORD nSize /*= 32767*/)
{
	/*
	Parameters
	lpAppName [in]
	The name of the section in the initialization file.
	lpReturnedString [out]
	A pointer to a buffer that receives the key name and value pairs associated with the named section. The buffer is filled with one or more null-terminated strings; the last string is followed by a second null character.
	nSize [in]
	The size of the buffer pointed to by the lpReturnedString parameter, in characters. The maximum profile section size is 32,767 characters.
	lpFileName [in]
	The name of the initialization file. If this parameter does not contain a full path to the file, the system searches for the file in the Windows directory.
	*/
	if (nSize > _MaxString) nSize = _MaxString;
	TCHAR tcValue[_MaxString];
	TCHAR tcTemp[255];
	VString vTemp;

	GetPrivateProfileSection(szSection, tcValue, nSize, m_szProfilePath);
	for (int i = 0, j = 0; i < (int)nSize; i++, j++)
	{
		if ((tcTemp[j] = tcValue[i]) == _T('\0'))
		{
			j = -1;
			vTemp.push_back(tcTemp);
		}
	}
	return vTemp;
}

void ProfileManager::WriteSection(LPCTSTR szSection, VString& vTemp)
{
	/*
	Parameters
	lpAppName [in]
	The name of the section in which data is written. This section name is typically the name of the calling application.
	lpString [in]
	The new key names and associated values that are to be written to the named section. This string is limited to 65,535 bytes.
	lpFileName [in]
	The name of the initialization file. If this parameter does not contain a full path for the file, the function searches the Windows directory for the file. If the file does not exist and lpFileName does not contain a full path, the function creates the file in the Windows directory.
	If the file exists and was created using Unicode characters, the function writes Unicode characters to the file. Otherwise, the function creates a file using ANSI characters.
	*/

	TCHAR tcValue[_MaxString];
	CString szTemp;
	int sum = 0;
	for (c_Vit i = vTemp.begin(); i != vTemp.end(); ++i)
	{
		szTemp = *i;
		for (int j = 0; j < szTemp.GetLength(); j++)
		{
			tcValue[sum + j] = szTemp.GetAt(j); 
		}
		sum += szTemp.GetLength();
		tcValue[sum++] = _T('\0');
	}
	tcValue[sum+1] = _T('\0');	
//	WritePrivateProfileSection(szSection, tcValue, _T("D:/000/桌面/test1.txt"));
	WritePrivateProfileSection(szSection, tcValue, m_szProfilePath);
}

BOOL ProfileManager::ReadLine(LPCTSTR szSection, CString& szLine)
{
	VString vTemp;
	vTemp = ReadSection(szSection);
	if (m_iCurLine == 0)	// read first line
	{
		m_szLastSection = szSection;
	}

	if ((szSection != m_szLastSection))	// read the same section and under vTemp of section size
	{
		m_iCurLine = 0;
	}

	else if(m_iCurLine == vTemp.size())
	{
		m_iCurLine = 0;
		return FALSE;
	}
	szLine = vTemp.at(m_iCurLine++);
	return TRUE;
}

void ProfileManager::WriteLine(LPCTSTR szSection, CString szLine)
{
	TCHAR tcValue[_MaxString];
	DWORD len = GetPrivateProfileSection(szSection, tcValue, _MaxString, _T("D:/000/桌面/test1.txt"));
	int iSize = szLine.GetLength();
	// imitate the effect of CStdioFile.WriteString(_T("\n"));
	for (int i = 0; i < iSize; ++i)
	{
		tcValue[len + i] = szLine.GetAt(i);
	}
	tcValue[len + iSize] = tcValue[len + iSize + 1] = _T('\0');

	WritePrivateProfileSection(szSection, tcValue, _T("D:/000/桌面/test1.txt"));	// _T("D:/000/桌面/test1.txt")
}

void ProfileManager::DeleteSection(LPCTSTR szSection)
{
	/*
	lpszSection [in]
	The name of the section to which the string will be copied. If the section does not exist, it is created. The name of the section is case independent, the string can be any combination of uppercase and lowercase letters.
	lpszKey [in]
	The name of the key to be associated with a string. If the key does not exist in the specified section, it is created. If this parameter is NULL, the entire section, including all keys and entries within the section, is deleted.
	lpStruct [in]
	The data to be copied. If this parameter is NULL, the key is deleted.
	uSizeStruct [in]
	The size of the buffer pointed to by the lpStruct parameter, in bytes.
	szFile [in]
	The name of the initialization file. If this parameter is NULL, the information is copied into the Win.ini file.
	If the file was created using Unicode characters, the function writes Unicode characters to the file. Otherwise, the function writes ANSI characters.
	*/
	WritePrivateProfileStruct(szSection, NULL, NULL, NULL, m_szProfilePath);
}

void ProfileManager::DeleteKeyName(LPCTSTR szSection, LPCTSTR szKeyName)
{
	WritePrivateProfileStruct(szSection, szKeyName, NULL, NULL, m_szProfilePath);
}

int ProfileManager::ReadInt(LPCTSTR szSection, LPCTSTR szKeyName, int iValue /* = 0 */)
{
	return GetPrivateProfileInt(szSection, szKeyName, iValue, m_szProfilePath);
}

void ProfileManager::WriteInt(LPCTSTR szSection, LPCTSTR szKeyName, int iValue)
{
	CString szTemp;
	szTemp.Format(_T("%d"), iValue);
	WritePrivateProfileString(szSection, szKeyName, szTemp, m_szProfilePath);
}

double ProfileManager::ReadDouble(LPCTSTR szSection, LPCTSTR szKeyName, double dValue /* = 0.0 */)
{
	CString szTemp; 
	szTemp = ReadString(szSection, szKeyName, _T(""));
	return _wtof(szTemp);
}

void ProfileManager::WriteDouble(LPCTSTR szSection, LPCTSTR szKeyName, double dValue)
{
	CString szTemp;
	szTemp.Format(_T("%f"), dValue);
	WriteString(szSection, szKeyName, szTemp);
}

CString ProfileManager::ReadString(LPCTSTR szSection, LPCTSTR szKeyName, LPCTSTR szValue /* = _T("") */)
{
	CString szTemp;
	GetPrivateProfileString(szSection, szKeyName, szValue, szTemp.GetBuffer(255), 255, m_szProfilePath);
	szTemp.ReleaseBuffer();
	return szTemp;
}

void ProfileManager::WriteString(LPCTSTR szSection, LPCTSTR szKeyName, LPCTSTR szValue)
{
	WritePrivateProfileString(szSection, szKeyName, szValue, m_szProfilePath);
}

void ProfileManager::ClearProfile()
{
	
}