#include "stdafx.h"
#include "GlobalFunction.h"

#include "Iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib") 
#pragma comment(lib, "version.lib")

//全局函数
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
	CString ExePath, ExeName;
	TCHAR tempPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,tempPath,MAX_PATH);
	ExePath = tempPath;
	//NOTICE: 得到的路径最后带 *\*
	ExeName  = ExePath.Right(ExePath.GetLength() - ExePath.ReverseFind(_T('\\'))-1);
	ExeName.Replace(L".exe", L"");
	return ExeName;
}

CString GetFileDir(CString FilePath)
{
	//NOTICE: 得到的路径最后带 *\*
	return FilePath.Left(FilePath.ReverseFind(_T('\\'))+1);
}

CString GetFileInfo(LPCTSTR strFilePath, LPCTSTR strItem)
{
	DWORD dwSize; 
	DWORD dwRtn; 
	CString szVersion;

	// 获取版本信息大小
	dwSize = ::GetFileVersionInfoSize(strFilePath, NULL); 
	if (dwSize == 0) 
	{ 
		return _T(""); 
	}

	char *pBuf = new char[dwSize + 1]; 
	memset(pBuf, 0, dwSize + 1);
	// 获取版本信息
	dwRtn = GetFileVersionInfo(strFilePath, NULL, dwSize, pBuf); 
	if(dwRtn == 0) 
	{ 
		delete [] pBuf;
		return _T(""); 
	}

	LPVOID lpBuffer = NULL;
	UINT uLen = 0;
	// 版本资源中获取信息
	CString str;
	str.Format(_T("\\StringFileInfo\\080404b0\\%s"),strItem);		
	//0804中文
		//04b0即1252,ANSI
		//可以从ResourceView中的Version中BlockHeader中看到
		//可以测试的属性
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
		if(!(attribute & dwRet)) // 如果它不是attribute 
		{ 
			if(INVALID_FILE_ATTRIBUTES == ::SetFileAttributes(tcName, dwRet | attribute)) 
			{ 
				return FALSE; 
			}
			return TRUE; 
		} 

		return TRUE; // 如果它已经是attribute的，就当设置成功了
	}
	else
	{
		if(attribute & dwRet) // 如果它是attribute的 
		{ 
			if(INVALID_FILE_ATTRIBUTES == ::SetFileAttributes(tcName, dwRet & ~attribute)) 
			{ 
				return FALSE; 
			}
			return TRUE; 
		} 

		return TRUE; // 如果它已经不是attribute的，就当设置成功了 
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
		dwCnt = MAXSIZE;      //坑,这个不能小于实际读出的值,否则返回234错误.
		dwNameCnt = MAXSIZE;
		memset(CommID, 0, MAXSIZE);
		memset(keyName, 0, MAXSIZE);
		ret = RegEnumValue(hkey, 
			commIndex, //第几个键
			keyName,         //保存键的名字字符串指针
			&dwNameCnt, //读入多少个字符串
			NULL,
			&dwType,        //REG_SZ类型还是 REG_WORD,等等
			(LPBYTE)CommID, //保存键值字符串指针
			&dwCnt);               //读入多少个字符串

		commIndex++;

		if (ret == ERROR_SUCCESS)
		{
			CString str;
			if(dwCnt>10) //TCHAR
				str.Format(_T("\\\\.\\%s"),CommID);  //COM号大于9
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
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //判断是否已','结尾
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//如果最后一个不是分割字符，再把后面的一段保存再退出
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
				strlist->push_back(str);
			}
			break;//如果最后一个就是分割字符，退出
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
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
	//为适配器申请内存
	::GetAdaptersInfo(adp,&uLong);
	adp = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR,uLong);

	//取得本地适配器结构信息
	if(::GetAdaptersInfo(adp,&uLong) == ERROR_SUCCESS)
	{
		if(adp != NULL)
		{
			//strMacAdd.Format("%s",adp->Address);
			memcpy(pMac,adp->Address,6);
			strMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),pMac[0],pMac[1],pMac[2],pMac[3],pMac[4],pMac[5]);
//			strGateWay.Format("%s",adp->GatewayList.IpAddress.String);// 网关
//			strIP.Format("%s",adp->IpAddressList.IpAddress.String);//IP
//			strSubnetMask.Format("%s",adp->IpAddressList.IpMask.String);//子网掩码
		}
	}

	return strMac;
}

CString GetIP(void)
{
	CString strIP;
	PIP_ADAPTER_INFO adp = NULL;
	ULONG uLong=0;
	//为适配器申请内存
	::GetAdaptersInfo(adp,&uLong);
	adp = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR,uLong);

	//取得本地适配器结构信息
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
	//为适配器申请内存
	::GetAdaptersInfo(adp,&uLong);
	adp = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR,uLong);

	//取得本地适配器结构信息
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

int GetWinVersion(CString& szVer, CString& szEdition)	// 获取操作系统的名称和版本
{
	// 得到当前Windows版本
	SYSTEM_INFO sysInfo;                // 用SYSTEM_INFO结构判断64位AMD处理器  
	GetSystemInfo(&sysInfo);            // 调用GetSystemInfo函数填充结构  

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
	AfxMessageBox(_T("程序即将重启"));
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
	FILETIME CreateTime;// 文件创建的时间
	FILETIME VisitTime;//文件访问时间
	FILETIME ModifyTime;// 文件修改时间
	FILETIME LocalTime;//本地时间
	BOOL bGotFileCreateTime;
	HANDLE hOriginal = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hOriginal)
	{
		printf("打开文件失败\n");
	}
	else
	{
		bGotFileCreateTime = GetFileTime( hOriginal, &CreateTime, &VisitTime, &ModifyTime );
		SYSTEMTIME tModify;
		FileTimeToLocalFileTime(&ModifyTime,&LocalTime);//刚获取到的时间是0时区的标准时间，先转换到本地时间
		FileTimeToSystemTime(&LocalTime,&tModify);//转换为SYSTEMTIME

	//	szTemp.Format(L"%d%02d%02d %d:%d:%d:%d",tModify.wYear,tModify.wMonth,tModify.wDay,tModify.wHour,tModify.wMinute,tModify.wSecond,tModify.wMilliseconds);
		szTemp.Format(L"%d%02d%02d",tModify.wYear,tModify.wMonth,tModify.wDay);
	}
	CloseHandle(hOriginal);
	return szTemp;
}