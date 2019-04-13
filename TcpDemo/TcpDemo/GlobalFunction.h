#pragma once
#include <vector>
using namespace std;
//全局函数 
CString GetExePath();	// 得到程序全路径
//全局函数 得到程序所在的文件夹【得到的路径最后带 \】
CString GetExeDir();
CString GetExePath();
CString GetExeName();
//得到文件版本信息等
CString GetFileInfo(LPCTSTR strFilePath, LPCTSTR strItem);
//拆分字符串
void	SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);
//合并字符串
CString MergeString(vector<CString>* strlist, TCHAR charSepareate);
//读出当前可用的串口编号
void	QueryComm(std::vector<CString>* vCommArray);
//得到主机的MAC
CString GetHostMAC(void);
//高精度延时
void	WaitTime(unsigned short msec);
//得到本机IP 字符串
CString GetIP(void);
//得到本机IP 整形
int	GetIP_int(void);
void ReStart(BOOL bNormal);
void ReplaceRightSlashToLeft(CString& szTemp);
void AddLeftSlash(CString& szTemp);
void AddRightSlash(CString& szTemp);

enum eWinVer // Windows OS Version
{
	Windows_Unknown,
	Windows_95,		// 4.0
	Windows_98,		// 4.10
	Windows_Me,		// 4.90
	Windows_NT351,	// 3
	Windows_NT40,	// 4.0
	Windows_2000,	// 5.0
	Windows_XP,		// 5.1
	Windows_XP_Professional_x64_Edition, // 5.2 (OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION) && (SYSTEM_INFO.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
	Windows_Server_2003,	// 5.2 GetSystemMetrics(SM_SERVERR2) == 0
	Windows_Home_Server,	// 5.2 OSVERSIONINFOEX.wSuiteMask & VER_SUITE_WH_SERVER
	Windows_Server_2003_R2, // 5.2 GetSystemMetrics(SM_SERVERR2) != 0
	Windows_Vista,			// 6.0 OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
	Windows_Server_2008,	// 6.0 OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
	Windows_7,				// 6.1 OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
	Windows_Server_2008_R2, // 6.1 OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
	Windows_8,				// 6.2 OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
	Windows_Server_2012,	// 6.2 OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
	Windows_8_1,			// 6.3 OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
	Windows_Server_2012_R2,	// 6.3 OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
	Windows_10,				// 10.0 OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
	Windows_Server_Technical_Preview, // 10.0 OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
};
int GetWinVersion(CString& szVer, CString& szEdition);
BOOL SetFileAttribute(const TCHAR* tcName, DWORD attribute, BOOL bSet);
CString GetFileTimeInfo(CString szPath);
CString FindLocalFilePath(CString path);