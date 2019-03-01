#include "StdAfx.h"
#include "Global.h"


LPCTSTR GetExePath(void)
{	
	// 获取主程序所在路径, 存在path中
	static CString path;	
	GetModuleFileName(NULL, path.GetBuffer(MAX_PATH+1), MAX_PATH);
	path.ReleaseBuffer();
	
	//TRACE(path);
	path = path.Left(path.ReverseFind('\\') + 1);
	
	return path;
}

LPCTSTR GetExeName(void)
{	
	CString path;	
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH+1), MAX_PATH);
	path.ReleaseBuffer();
	//TRACE(path);
	
	int start = path.ReverseFind(_T('\\')) + 1;
	int end   = path.ReverseFind(_T('.'));
	
	static CString name = path.Mid(start, end - start);	
	return name;
}

LPCTSTR GetCurrentTimeString(void)
{
	CTime time = CTime::GetCurrentTime();

	static CString str;
	str.Format(_T("%d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return str;
}

#define DAYS_OF_WEEK        7
#define MONTHS_OF_YEAR      12
#define isLeapYear(y)       (((y) % 4 == 0 && (y) % 100 != 0) || ((y) % 400 == 0))

#define BASE_YEAR           1
#define BASE_YEAR_FIRST     MONDAY

enum WEEKDAY 
{
    SUNDAY = 0,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
};

/* 计算一天在这年的第几周，一周以星期日为开始，输入年数不限制 */
UINT GetWeek(const UINT year, const UINT month, const UINT date)
{
	if ((year==0) || (month==0) || (date==0) || (month > MONTHS_OF_YEAR))
	{
		return 0;
	}
	
	const static UINT daysOfMonthCommon[MONTHS_OF_YEAR + 1] = 
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	const static UINT daysOfMonthLeap[MONTHS_OF_YEAR + 1] = 
	{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	const UINT *daysOfMonth;
	if (isLeapYear(year))
	{
		daysOfMonth = daysOfMonthLeap;
	}
	else 
	{
		daysOfMonth = daysOfMonthCommon;
	}

	if (date > daysOfMonth[month])
	{
		return 0;
	}
	
	/* 计算这天是这年的第几天 */
	UINT dayNum = 0;

	for (UINT i=0; i<month; i++) 
	{
		dayNum += daysOfMonth[i];
	}
	dayNum += date;
	
	/* 从公元BASE_YEAR年开始演算，得出这年第一天是星期几 */
	UINT first = BASE_YEAR_FIRST;  /* 一年第一天的星期数 */
	
	for (UINT j=BASE_YEAR; j<year; j++)
	{
		if (isLeapYear(j))
		{
			first += 366 % DAYS_OF_WEEK; // 闰年有366天，非闰年365天
		} 
		else 
		{
			first += 365 % DAYS_OF_WEEK;
		}
	}
	first = first % DAYS_OF_WEEK;
	
    /* 换算为从0开始而非从1开始再计算 */
	//ret = (dayNum - 1 + first) % DAYS_OF_WEEK;      // 计算这天为星期几
	//printf("weekday %d\n", ret);

	UINT week = (dayNum - 1 + first) / DAYS_OF_WEEK + 1;  // 计算这天为这年的第几周
	
	return week;
}

#include <nb30.h>
#pragma comment(lib, "Netapi32")

// 输入参数：lana_num为网卡编号，一般地，从0开始，但在Windows 2000中并不一定是连续分配的    
LPCTSTR GetMAC(int lana_num)
{   
	NCB ncb;
	memset(&ncb, 0, sizeof(NCB));   	
	ncb.ncb_command = NCBRESET;  //对指定网卡初始化
	ncb.ncb_lana_num = lana_num;

	UCHAR uRetCode;
	uRetCode = Netbios(&ncb);
	if (uRetCode != 0)
	{
		return _T("");
	}

	typedef struct _ASTAT_   
	{   
		ADAPTER_STATUS adapt;   
		NAME_BUFFER NameBuff[30];   
	}ASTAT, *PASTAT;   
	
	ASTAT Adapter;       

	memset(&ncb, 0, sizeof(NCB));
	ncb.ncb_command = NCBASTAT;  // 设定查询指定网卡
	ncb.ncb_lana_num = lana_num;	
	ncb.ncb_buffer = (unsigned char*)&Adapter;  // 指定返回信息的存储内存
	ncb.ncb_length = sizeof(Adapter);

	strcpy((char*)ncb.ncb_callname, "*" );  // 很重要

	// 查询网卡
	uRetCode = Netbios(&ncb);
	if (uRetCode != 0)
	{
		return _T("");
	}

	// 获取网卡16进制格式 00-10-AA-E4-5B-02
	static CString MAC;
	MAC.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);

	return MAC;
}

LPCTSTR GetFirstMAC(void)
{
    LANA_ENUM lana_enum;
	
    NCB ncb;
    memset(&ncb, 0, sizeof(NCB));
    ncb.ncb_command = NCBENUM;  // 指定指令为NCBENUM, 用于获取网卡的数量及编号  
    ncb.ncb_buffer = (unsigned char*)&lana_enum;  // 指定返回的信息存贮内存
    ncb.ncb_length = sizeof(LANA_ENUM);
	
    // 向网卡查询来获取网卡的信息
    UCHAR uRetCode = Netbios(&ncb);
    if (uRetCode != 0)
    {
		return _T("");
	}

    //TRACE(_T("网卡数量: %d个\n"), lana_enum.length);
	
    // 获取第一个网卡的地址
    return GetMAC(lana_enum.lana[0]);
}

// 本函数来获取所有的网卡
void GetAllMAC(void)
{
    LANA_ENUM lana_enum;
	
    NCB ncb;
    memset(&ncb, 0, sizeof(NCB));
    ncb.ncb_command = NCBENUM;  // 指定指令为NCBENUM, 用于获取网卡的数量及编号  
    ncb.ncb_buffer = (unsigned char*)&lana_enum;  // 指定返回的信息存贮内存
    ncb.ncb_length = sizeof(LANA_ENUM);
	
    // 向网卡查询来获取网卡的信息
    UCHAR uRetCode = Netbios(&ncb);
    if (uRetCode == 0)
    {
        //TRACE(_T("网卡数量: %d个\n"), lana_enum.length);
		
        // 获取每个网卡的地址
        for (int loopi=0; loopi<lana_enum.length; loopi++)
        {
            GetMAC(lana_enum.lana[loopi]);
        }
    }
}

void ExecuteOpenFile(LPCTSTR filePath)
{
	ShellExecute(NULL, _T("open"), filePath, NULL, NULL, SW_SHOWNORMAL);	
}

BYTE* ReadBinaryFile(LPCTSTR filePath)
{	
	if (filePath == _T(""))
	{
		return NULL;
	}
	
	CFile file;
	if (!file.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(_T("Failed to Open file!"));
		return NULL;
	}
	
	ULONG length = (ULONG)file.GetLength();

	BYTE *buffer = new BYTE[length]; 
	if (buffer == NULL)
	{
		//TRACE(_T("Memory Allocate ERROR!\n"));
		file.Close();
		return NULL;
	}
	
	file.Read(buffer, length);
	file.Close();
	
	return buffer;
}

BOOL SaveBinaryFile(LPCTSTR filePath, BYTE *data, const LONG length)
{
	if (filePath == _T(""))
	{
		return FALSE;
	}
	
	CFile file;
	if (!file.Open(filePath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		AfxMessageBox(_T("Failed to Open file!"));
		return FALSE;
	}

	file.Write(data, length);
	file.Close();

	return TRUE;
}

BOOL SaveFileCSV(LPCTSTR filePath, LPCTSTR headers, LPCTSTR contents)
{
	if (filePath == _T(""))
	{		
		return FALSE;
	//	filePath = GetExePath() + GetCurrentTimeString() + _T(".csv");
	}
	
	CStdioFile file;
	CFileFind fileFind;
	
	if (fileFind.FindFile(filePath))
	{
		if (!file.Open(filePath, CFile::modeRead | CFile::modeWrite))
		{
			CString info;
			info.Format(_T("Open %s Error!"), filePath);
			AfxMessageBox(info);
			
			return FALSE;
		}
	}
	else
	{
		if (!file.Open(filePath, CFile::modeCreate | CFile::modeRead | CFile::modeWrite))	
		{
			CString info;
			info.Format(_T("Open %s Error!"), filePath);
			AfxMessageBox(info);
			
			return FALSE;
		}
		
		file.SeekToEnd();		
		file.WriteString(headers);		
		file.WriteString(_T("\n"));
	}
	
	file.SeekToEnd();	
	
	file.WriteString(contents);
	file.WriteString(_T("\n"));
	
	file.Close();
	
	return TRUE;
}

BOOL WriteFileString(LPCTSTR filePath, LPCTSTR contents)
{
	if (filePath == _T(""))
	{	
		return FALSE;
	}
	
	CStdioFile file;
	CFileFind fileFind;
	
	if (fileFind.FindFile(filePath))
	{
		if (!file.Open(filePath, CFile::modeRead | CFile::modeWrite))
		{
			CString info;
			info.Format(_T("Open %s Error!"), filePath);
			AfxMessageBox(info);
			
			return FALSE;
		}
	}
	else
	{
		if (!file.Open(filePath, CFile::modeCreate | CFile::modeRead | CFile::modeWrite))	
		{
			CString info;
			info.Format(_T("Open %s Error!"), filePath);
			AfxMessageBox(info);
			
			return FALSE;
		}
		
	//	file.SeekToEnd();		
	//	file.WriteString(_T("\n"));
	}
	
	file.SeekToEnd();	
	
	file.WriteString(contents);
//	file.WriteString(_T("\n"));
	
	file.Close();
	
	return TRUE;
}

LPSTR LPWSTRtoLPSTR(LPWSTR strIn)
{
	LPSTR strOut = NULL;

	if (strIn != NULL)
	{
		int lengthIn = wcslen(strIn);
		int lengthOut = WideCharToMultiByte(CP_ACP, 0, strIn, lengthIn, NULL, 0, NULL, NULL);

		strOut = new char[lengthOut+1];
		if (strOut)
		{
			WideCharToMultiByte(CP_ACP, 0, strIn, lengthIn, strOut, lengthOut, NULL, NULL);
			strOut[lengthOut] = '\0';
		}		 
	}

	return strOut;
}

BOOL CheckEqual(BYTE *data1, BYTE *data2, int length)
{
	if (data1 == NULL || data2 == NULL)
	{
		return FALSE;
	}
	
	if (length <= 0)
	{
		return FALSE;
	}	

	for (int i=0; i<length; i++)
	{
		if (data1[i] != data2[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}

int CalcCheckSum(BYTE *data, int length, int mod)
{
//	ASSERT(data != NULL);
	if (data == NULL)
	{
		return 0;
	}
	
	ULONG  sum = 0;
	
	for (int i=0; i<length; i++)
	{
		sum += data[i];
	}
	
	return sum % mod;
}
