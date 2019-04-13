#pragma once
#include <vector>
using namespace std;
//ȫ�ֺ��� 
CString GetExePath();	// �õ�����ȫ·��
//ȫ�ֺ��� �õ��������ڵ��ļ��С��õ���·������ \��
CString GetExeDir();
CString GetExePath();
CString GetExeName();
//�õ��ļ��汾��Ϣ��
CString GetFileInfo(LPCTSTR strFilePath, LPCTSTR strItem);
//����ַ���
void	SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);
//�ϲ��ַ���
CString MergeString(vector<CString>* strlist, TCHAR charSepareate);
//������ǰ���õĴ��ڱ��
void	QueryComm(std::vector<CString>* vCommArray);
//�õ�������MAC
CString GetHostMAC(void);
//�߾�����ʱ
void	WaitTime(unsigned short msec);
//�õ�����IP �ַ���
CString GetIP(void);
//�õ�����IP ����
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