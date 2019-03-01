#pragma once

#include <atlstr.h>	// for declaration of CString and LPCTSTR

class CsvManager
{
public:
	CsvManager(void);
	~CsvManager(void);

private:
	CString m_filePath;
	CString m_fileName;


public:
	// 
	BOOL SetColumn();
	BOOL SetRow();
	int  GetColumn();
	int  GetRow();

	BOOL AddColumn(LPCTSTR col = _T(""));
	BOOL AddRow(LPCTSTR row = _T(""));
};

