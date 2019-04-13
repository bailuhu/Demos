#include "stdafx.h"
#include "GlobalFunction.h"

#include "Iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib") 
#pragma comment(lib, "version.lib")

//ȫ�ֺ���
CString GetExeDir()
{
	CString ExePath, ExeDir;
	TCHAR tempPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,tempPath,MAX_PATH);
	ExePath = tempPath;
	//NOTICE: �õ���·������ *\*
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
	CString ExePath, ExeName;
	TCHAR tempPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,tempPath,MAX_PATH);
	ExePath = tempPath;
	//NOTICE: �õ���·������ *\*
	ExeName  = ExePath.Right(ExePath.GetLength() - ExePath.ReverseFind(_T('\\'))-1);
	ExeName.Replace(L".exe", L"");
	return ExeName;
}

CString GetFileDir(CString FilePath)
{
	//NOTICE: �õ���·������ *\*
	return FilePath.Left(FilePath.ReverseFind(_T('\\'))+1);
}

CString GetFileInfo(LPCTSTR strFilePath, LPCTSTR strItem)
{
	DWORD dwSize; 
	DWORD dwRtn; 
	CString szVersion;

	// ��ȡ�汾��Ϣ��С
	dwSize = ::GetFileVersionInfoSize(strFilePath, NULL); 
	if (dwSize == 0) 
	{ 
		return _T(""); 
	}

	char *pBuf = new char[dwSize + 1]; 
	memset(pBuf, 0, dwSize + 1);
	// ��ȡ�汾��Ϣ
	dwRtn = GetFileVersionInfo(strFilePath, NULL, dwSize, pBuf); 
	if(dwRtn == 0) 
	{ 
		delete [] pBuf;
		return _T(""); 
	}

	LPVOID lpBuffer = NULL;
	UINT uLen = 0;
	// �汾��Դ�л�ȡ��Ϣ
	CString str;
	str.Format(_T("\\StringFileInfo\\080404b0\\%s"),strItem);		
	//0804����
		//04b0��1252,ANSI
		//���Դ�ResourceView�е�Version��BlockHeader�п���
		//���Բ��Ե�����
		/*
		CompanyName 
		FileDescription 
		FileVersion 
		InternalName 
		LegalCopyright
		OriginalFilename
		ProductName 
		ProductVersion 
		Comments
		LegalTrademarks 
		PrivateBuild 
		SpecialBuild 
		*/ 
	dwRtn = VerQueryValue(pBuf, str, &lpBuffer, &uLen); 
	if(dwRtn == 0) 
	{ 
		delete [] pBuf;
		return _T(""); 
	}

	TCHAR* p = new TCHAR[uLen+1];
	p[uLen] = 0;
	memcpy(p,lpBuffer,uLen*sizeof(TCHAR));

	szVersion = (TCHAR*)p;

	delete [] pBuf; 
	delete [] p;
	return szVersion; 
}

BOOL SetFileAttribute(const TCHAR* tcName, DWORD attribute, BOOL bSet) 
{ 
	DWORD dwRet = ::GetFileAttributes(tcName); 
	if( INVALID_FILE_ATTRIBUTES == dwRet ) return false; 

	if (bSet)
	{
		if(!(attribute & dwRet)) // ���������attribute 
		{ 
			if(INVALID_FILE_ATTRIBUTES == ::SetFileAttributes(tcName, dwRet | attribute)) 
			{ 
				return FALSE; 
			}
			return TRUE; 
		} 

		return TRUE; // ������Ѿ���attribute�ģ��͵����óɹ���
	}
	else
	{
		if(attribute & dwRet) // �������attribute�� 
		{ 
			if(INVALID_FILE_ATTRIBUTES == ::SetFileAttributes(tcName, dwRet & ~attribute)) 
			{ 
				return FALSE; 
			}
			return TRUE; 
		} 

		return TRUE; // ������Ѿ�����attribute�ģ��͵����óɹ��� 
	}
} 

void QueryComm(vector<CString>* vCommArray)
{
	const int MAXSIZE = 255;
	DWORD dwType;
	DWORD dwCnt, dwNameCnt;
	DWORD commIndex = 0;
	TCHAR  keyName[MAXSIZE];
	TCHAR  CommID[MAXSIZE];


	HKEY hkey;
	long ret = RegOpenKey(
		HKEY_LOCAL_MACHINE,
		_T( "HARDWARE\\DEVICEMAP\\SERIALCOMM"),
		&hkey);


	while ( ret == ERROR_SUCCESS )
	{		
		dwCnt = MAXSIZE;      //��,�������С��ʵ�ʶ�����ֵ,���򷵻�234����.
		dwNameCnt = MAXSIZE;
		memset(CommID, 0, MAXSIZE);
		memset(keyName, 0, MAXSIZE);
		ret = RegEnumValue(hkey, 
			commIndex, //�ڼ�����
			keyName,         //������������ַ���ָ��
			&dwNameCnt, //������ٸ��ַ���
			NULL,
			&dwType,        //REG_SZ���ͻ��� REG_WORD,�ȵ�
			(LPBYTE)CommID, //�����ֵ�ַ���ָ��
			&dwCnt);               //������ٸ��ַ���

		commIndex++;

		if (ret == ERROR_SUCCESS)
		{
			CString str;
			if(dwCnt>10) //TCHAR
				str.Format(_T("\\\\.\\%s"),CommID);  //COM�Ŵ���9
			else
			    str = CommID;
			vCommArray->push_back(str);
		}
	} 	
	RegCloseKey(hkey);
}

void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
{
	if(SrcString == _T("")) return;
	strlist->clear();
	CString temp, str;
	int nStart = 0, index = 0;
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //�ж��Ƿ���','��β
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//������һ�����Ƿָ��ַ����ٰѺ����һ�α������˳�
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //���Ϊ�գ�Ĭ�ϡ�0��,��ֹ���ݿ��ַ���Ϊ�ճ���
				strlist->push_back(str);
			}
			break;//������һ�����Ƿָ��ַ����˳�
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //���Ϊ�գ�Ĭ�ϡ�0��,��ֹ���ݿ��ַ���Ϊ�ճ���
		strlist->push_back(str);
		nStart = index + 1;
	}
}

CString MergeString(vector<CString>* strlist, TCHAR charSepareate)
{
	CString temp;
	for (UINT i=0; i<strlist->size(); i++)
	{
		temp += strlist->at(i) + charSepareate;
	}
	return temp;
}

CString GetHostMAC(void)
{
	CString strIP,strGateWay,strSubnetMask,strMac;
	u_char pMac[6];
	PIP_ADAPTER_INFO adp = NULL;
	ULONG uLong=0;
	//Ϊ�����������ڴ�
	::GetAdaptersInfo(adp,&uLong);
	adp = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR,uLong);

	//ȡ�ñ����������ṹ��Ϣ
	if(::GetAdaptersInfo(adp,&uLong) == ERROR_SUCCESS)
	{
		if(adp != NULL)
		{
			//strMacAdd.Format("%s",adp->Address);
			memcpy(pMac,adp->Address,6);
			strMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),pMac[0],pMac[1],pMac[2],pMac[3],pMac[4],pMac[5]);
//			strGateWay.Format("%s",adp->GatewayList.IpAddress.String);// ����
//			strIP.Format("%s",adp->IpAddressList.IpAddress.String);//IP
//			strSubnetMask.Format("%s",adp->IpAddressList.IpMask.String);//��������
		}
	}

	return strMac;
}

CString GetIP(void)
{
	CString strIP;
	PIP_ADAPTER_INFO adp = NULL;
	ULONG uLong=0;
	//Ϊ�����������ڴ�
	::GetAdaptersInfo(adp,&uLong);
	adp = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR,uLong);

	//ȡ�ñ����������ṹ��Ϣ
	if(::GetAdaptersInfo(adp,&uLong) == ERROR_SUCCESS)
	{
		if(adp != NULL)
		{
			strIP = adp->IpAddressList.IpAddress.String;
		}
	}
	return strIP;
}

int GetIP_int(void)
{
	int IP;
	PIP_ADAPTER_INFO adp = NULL;
	ULONG uLong=0;
	//Ϊ�����������ڴ�
	::GetAdaptersInfo(adp,&uLong);
	adp = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR,uLong);

	//ȡ�ñ����������ṹ��Ϣ
	if(::GetAdaptersInfo(adp,&uLong) == ERROR_SUCCESS)
	{
		if(adp != NULL)
		{
			IP = inet_addr(adp->IpAddressList.IpAddress.String);
		}
	}
	return IP;
}

void WaitTime(unsigned short msec)
{
	// To call your Wait function here
	LARGE_INTEGER liFreq;
	LARGE_INTEGER liStart, liEnd;
	LONGLONG llTime = 0;
	QueryPerformanceFrequency(&liFreq);
	QueryPerformanceCounter(&liStart);
	while(1)
	{
		QueryPerformanceCounter(&liEnd);
		llTime = 1000000 * (liEnd.QuadPart - liStart.QuadPart) / liFreq.QuadPart;
		if(llTime > msec * 1000)
		{
			break;
		}
	}
}

int GetWinVersion(CString& szVer, CString& szEdition)	// ��ȡ����ϵͳ�����ƺͰ汾
{
	// �õ���ǰWindows�汾
	SYSTEM_INFO sysInfo;                // ��SYSTEM_INFO�ṹ�ж�64λAMD������  
	GetSystemInfo(&sysInfo);            // ����GetSystemInfo�������ṹ  

	OSVERSIONINFOEX winVer;
	ZeroMemory(&winVer, sizeof(OSVERSIONINFOEX));
	winVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	DWORD majorVer;
	DWORD minorVer;
	DWORD platformId;

	if(GetVersionEx((OSVERSIONINFO*)&winVer))
	{
		majorVer	= winVer.dwMajorVersion;
		minorVer	= winVer.dwMinorVersion;
		platformId	= winVer.dwPlatformId;

		switch(majorVer)
		{
		case 4:
			switch(minorVer)
			{
			case 0:
				if (platformId == VER_PLATFORM_WIN32_WINDOWS) {szVer = _T("Windows 95");return Windows_95;}
			case 10:
				szVer = _T("Windows 98");
				return Windows_98;
			case 90:
				szVer = _T("Windows Me");
				return Windows_Me;
			}
		case 5:
			switch(minorVer)
			{
			case 0:
				if(winVer.wSuiteMask == VER_SUITE_ENTERPRISE)  szEdition.Format(_T("Advanced Server Edition %s"), winVer.szCSDVersion); 

				szVer = _T("Windows 2000");
				return Windows_2000;
			case 1:
				if(winVer.wSuiteMask == VER_SUITE_EMBEDDEDNT)	szEdition.Format(_T("Embedded Edition %s"), winVer.szCSDVersion); 
				else if(winVer.wSuiteMask == VER_SUITE_PERSONAL)  szEdition.Format(_T("Home Edition %s"), winVer.szCSDVersion); 
				else  szEdition.Format(_T("Professional Edition %s"), winVer.szCSDVersion); 

				szVer = _T("Windows XP");
				return Windows_XP;
			case 2:
				if((winVer.wProductType == VER_NT_WORKSTATION) && (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64))
				{
					szVer = _T("Windows XP Professional x64 Edition");
					return Windows_XP_Professional_x64_Edition;
				}
				if (GetSystemMetrics(SM_SERVERR2) == 0) 
				{
					switch(winVer.wSuiteMask)
					{
					case VER_SUITE_BLADE:			szEdition.Format(_T("Web Edition %s"), winVer.szCSDVersion); break;
					case VER_SUITE_COMPUTE_SERVER:	szEdition.Format(_T("Compute Cluster Edition %s"), winVer.szCSDVersion); break;
					case VER_SUITE_STORAGE_SERVER:	szEdition.Format(_T("Storage Server Edition %s"), winVer.szCSDVersion); break; 
					case VER_SUITE_DATACENTER:		szEdition.Format(_T("Datacenter Edition %s"), winVer.szCSDVersion); break;
					case VER_SUITE_ENTERPRISE:		szEdition.Format(_T("Enterprise Edition %s"), winVer.szCSDVersion); break;
					}

					szVer = _T("Windows Server 2003");
					return Windows_Server_2003;
				}
				else									
				{
					if(winVer.wSuiteMask == VER_SUITE_STORAGE_SERVER)   szEdition.Format(_T("Storage Server Edition %s"), winVer.szCSDVersion); 

					szVer = _T("Windows Server 2003 R2");
					return Windows_Server_2003_R2;
				}
				if (winVer.wSuiteMask = VER_SUITE_WH_SERVER){szVer = _T("Windows Home Server");return Windows_Home_Server;}
			}
		case 6:
			switch(minorVer)
			{
			case 0:
				if(winVer.wSuiteMask == VER_SUITE_ENTERPRISE)  szEdition.Format(_T("Enterprise Edition %s"), winVer.szCSDVersion); 
				if(winVer.wSuiteMask == VER_SUITE_DATACENTER)  szEdition.Format(_T("Datacenter Server Edition %s"), winVer.szCSDVersion); 

				if (winVer.wProductType == VER_NT_WORKSTATION)	
				{
					if(winVer.wSuiteMask == VER_SUITE_PERSONAL) szEdition.Format(_T("Home Edition %s"), winVer.szCSDVersion); 

					szVer = _T("Windows Vista");return Windows_Vista;
				}
				else											
				{
					if(winVer.wSuiteMask == VER_SUITE_ENTERPRISE)  szEdition.Format(_T("Enterprise Edition %s"), winVer.szCSDVersion); 
					if(winVer.wSuiteMask == VER_SUITE_DATACENTER)  szEdition.Format(_T("Datacenter Server Edition %s"), winVer.szCSDVersion); 

					szVer = _T("Windows Server 2008");
					return Windows_Server_2008;
				}
			case 1:
				if (winVer.wProductType == VER_NT_WORKSTATION)	{szVer = _T("Windows 7"); szEdition.Format(_T(" %s"), winVer.szCSDVersion); return Windows_7;}
				else											{szVer = _T("Windows Server 2008 R2");return Windows_Server_2008_R2;}
			case 2:
				if (winVer.wProductType == VER_NT_WORKSTATION)	{szVer = _T("Windows 8");return Windows_8;}
				else											{szVer = _T("Windows Server 2012");return Windows_Server_2012;}
			case 3:
				if (winVer.wProductType == VER_NT_WORKSTATION)	{szVer = _T("Windows 8.1");return Windows_8_1;}
				else											{szVer = _T("Windows Server 2012 R2");return Windows_Server_2012_R2;}
			}
		case 10:
			if (winVer.wProductType == VER_NT_WORKSTATION)	{szVer = _T("Windows 10");return Windows_10;}
			else											{szVer = _T("Windows Server Technical Preview");return Windows_Server_Technical_Preview;}
		default:
			szVer = _T("Unknow version!");
			szEdition.Format(_T(" Unknown Edition %s"), winVer.szCSDVersion); 
			return Windows_Unknown;
		}
	}
	return Windows_Unknown;
}

void ReStart(BOOL bNormal)
{
	AfxMessageBox(_T("���򼴽�����"));
	PROCESS_INFORMATION info;
	STARTUPINFO startup;
	TCHAR szPath[256];
	TCHAR *szCmdLine;
	GetModuleFileName(AfxGetApp()-> m_hInstance, szPath, sizeof(szPath));
	szCmdLine = GetCommandLine();
	GetStartupInfo(&startup);

 	BOOL bSucc = CreateProcess(szPath, szCmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup, &info);
 	if(bNormal && bSucc)
	{
		CWnd *pWnd = AfxGetMainWnd();
		if(pWnd != NULL)
			pWnd->PostMessage(WM_CLOSE, 0, 0);
		else
			ExitProcess(-1);
	}
	else
		ExitProcess(-1);

}

void ReplaceRightSlashToLeft(CString& szTemp)  
{ 
	szTemp.Replace(_T('\\'), _T('/')); 
}
void AddLeftSlash(CString& szTemp)
{// if there is no left slash, add it; otherwise don't
	if (szTemp.Right(1) != _T('/')) szTemp += _T('/');
}
void AddRightSlash(CString& szTemp)
{// if there is no right slash, add it; otherwise don't
	if (szTemp.Right(1) != _T('\\')) szTemp += _T('\\');
}

CString FindLocalFilePath(CString path)
{
	if (path.IsEmpty()) return FALSE;

	CFileFind finder;
	BOOL bw = finder.FindFile(path);
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (finder.IsDirectory()) continue;
		return finder.GetFilePath();
	}
	return L"";
}

CString GetFileTimeInfo(CString szPath)
{
	CString szTemp;
	FILETIME CreateTime;// �ļ�������ʱ��
	FILETIME VisitTime;//�ļ�����ʱ��
	FILETIME ModifyTime;// �ļ��޸�ʱ��
	FILETIME LocalTime;//����ʱ��
	BOOL bGotFileCreateTime;
	HANDLE hOriginal = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hOriginal)
	{
		printf("���ļ�ʧ��\n");
	}
	else
	{
		bGotFileCreateTime = GetFileTime( hOriginal, &CreateTime, &VisitTime, &ModifyTime );
		SYSTEMTIME tModify;
		FileTimeToLocalFileTime(&ModifyTime,&LocalTime);//�ջ�ȡ����ʱ����0ʱ���ı�׼ʱ�䣬��ת��������ʱ��
		FileTimeToSystemTime(&LocalTime,&tModify);//ת��ΪSYSTEMTIME

	//	szTemp.Format(L"%d%02d%02d %d:%d:%d:%d",tModify.wYear,tModify.wMonth,tModify.wDay,tModify.wHour,tModify.wMinute,tModify.wSecond,tModify.wMilliseconds);
		szTemp.Format(L"%d%02d%02d",tModify.wYear,tModify.wMonth,tModify.wDay);
	}
	CloseHandle(hOriginal);
	return szTemp;
}