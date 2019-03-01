#include "stdafx.h"
#include "GlobalFunc.h"

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

//全局函数
CString GetResDir()
{
	CString ExePath, ExeDir;
	TCHAR tempPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,tempPath,MAX_PATH);
	ExePath = tempPath;
	//NOTICE: 得到的路径最后带 *\*
	ExeDir  = ExePath.Left(ExePath.ReverseFind(_T('\\')));
	ExeDir  = ExeDir.Left(ExeDir.ReverseFind(_T('\\'))+1);
	return ExeDir + _T("resource\\");
}

CString GetExeDir()
{
	CString ExePath, ExeDir;
	TCHAR tempPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,tempPath,MAX_PATH);
	ExePath = tempPath;
	//NOTICE: 得到的路径最后带 *\*
	ExeDir  = ExePath.Left( ExePath.ReverseFind(_T('\\'))+1) ;
	return ExeDir;
}

CString GetExePath()
{
	CString ExePath;
	TCHAR tempPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, tempPath, MAX_PATH);
	ExePath = tempPath;
	return ExePath;
}

CString GetExeName()
{
	CString ExeName;
	TCHAR tempPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, tempPath, MAX_PATH);
	ExeName = tempPath;
	ExeName = ExeName.Right(ExeName.GetLength() - ExeName.ReverseFind(_T('\\'))-1);
	ExeName = ExeName.Left(ExeName.ReverseFind(_T('.'))-1);
	return ExeName;
}