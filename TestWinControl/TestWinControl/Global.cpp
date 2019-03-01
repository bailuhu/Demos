#include "StdAfx.h"
#include "Global.h"


LPCTSTR GetExePath(void)
{	
	// ��ȡ����������·��, ����path��
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

/* ����һ��������ĵڼ��ܣ�һ����������Ϊ��ʼ���������������� */
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
	
	/* ��������������ĵڼ��� */
	UINT dayNum = 0;

	for (UINT i=0; i<month; i++) 
	{
		dayNum += daysOfMonth[i];
	}
	dayNum += date;
	
	/* �ӹ�ԪBASE_YEAR�꿪ʼ���㣬�ó������һ�������ڼ� */
	UINT first = BASE_YEAR_FIRST;  /* һ���һ��������� */
	
	for (UINT j=BASE_YEAR; j<year; j++)
	{
		if (isLeapYear(j))
		{
			first += 366 % DAYS_OF_WEEK; // ������366�죬������365��
		} 
		else 
		{
			first += 365 % DAYS_OF_WEEK;
		}
	}
	first = first % DAYS_OF_WEEK;
	
    /* ����Ϊ��0��ʼ���Ǵ�1��ʼ�ټ��� */
	//ret = (dayNum - 1 + first) % DAYS_OF_WEEK;      // ��������Ϊ���ڼ�
	//printf("weekday %d\n", ret);

	UINT week = (dayNum - 1 + first) / DAYS_OF_WEEK + 1;  // ��������Ϊ����ĵڼ���
	
	return week;
}

#include <nb30.h>
#pragma comment(lib, "Netapi32")

// ���������lana_numΪ������ţ�һ��أ���0��ʼ������Windows 2000�в���һ�������������    
LPCTSTR GetMAC(int lana_num)
{   
	NCB ncb;
	memset(&ncb, 0, sizeof(NCB));   	
	ncb.ncb_command = NCBRESET;  //��ָ��������ʼ��
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
	ncb.ncb_command = NCBASTAT;  // �趨��ѯָ������
	ncb.ncb_lana_num = lana_num;	
	ncb.ncb_buffer = (unsigned char*)&Adapter;  // ָ��������Ϣ�Ĵ洢�ڴ�
	ncb.ncb_length = sizeof(Adapter);

	strcpy((char*)ncb.ncb_callname, "*" );  // ����Ҫ

	// ��ѯ����
	uRetCode = Netbios(&ncb);
	if (uRetCode != 0)
	{
		return _T("");
	}

	// ��ȡ����16���Ƹ�ʽ 00-10-AA-E4-5B-02
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
    ncb.ncb_command = NCBENUM;  // ָ��ָ��ΪNCBENUM, ���ڻ�ȡ���������������  
    ncb.ncb_buffer = (unsigned char*)&lana_enum;  // ָ�����ص���Ϣ�����ڴ�
    ncb.ncb_length = sizeof(LANA_ENUM);
	
    // ��������ѯ����ȡ��������Ϣ
    UCHAR uRetCode = Netbios(&ncb);
    if (uRetCode != 0)
    {
		return _T("");
	}

    //TRACE(_T("��������: %d��\n"), lana_enum.length);
	
    // ��ȡ��һ�������ĵ�ַ
    return GetMAC(lana_enum.lana[0]);
}

// ����������ȡ���е�����
void GetAllMAC(void)
{
    LANA_ENUM lana_enum;
	
    NCB ncb;
    memset(&ncb, 0, sizeof(NCB));
    ncb.ncb_command = NCBENUM;  // ָ��ָ��ΪNCBENUM, ���ڻ�ȡ���������������  
    ncb.ncb_buffer = (unsigned char*)&lana_enum;  // ָ�����ص���Ϣ�����ڴ�
    ncb.ncb_length = sizeof(LANA_ENUM);
	
    // ��������ѯ����ȡ��������Ϣ
    UCHAR uRetCode = Netbios(&ncb);
    if (uRetCode == 0)
    {
        //TRACE(_T("��������: %d��\n"), lana_enum.length);
		
        // ��ȡÿ�������ĵ�ַ
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
