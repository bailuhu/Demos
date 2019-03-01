#include "StdAfx.h"
#include "TimeManager.h"

CTimeManager::CTimeManager(void)
{

}


CTimeManager::~CTimeManager(void)
{
}

CString CTimeManager::GetCurDateAndTimeString(TCHAR tc /*= _T(':')*/)
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	if (tc == _T(''))
	{
		szTemp.Format(_T("%04d%02d%02d%02d%02d%02d"),
			m_curTime.GetYear(), 
			m_curTime.GetMonth(), 
			m_curTime.GetDay(),
			m_curTime.GetHour(),
			m_curTime.GetMinute(),
			m_curTime.GetSecond());
	}
	else
	{
		szTemp.Format(_T("%04d%c%02d%c%02d%c%02d%c%02d%c%02d"),
		          m_curTime.GetYear(), tc,
				  m_curTime.GetMonth(), tc,
				  m_curTime.GetDay(), tc,
				  m_curTime.GetHour(), tc,
				  m_curTime.GetMinute(), tc,
				  m_curTime.GetSecond());
	}
	return szTemp;
}

CString CTimeManager::GetCurTimeString(TCHAR tc /* = _T */)
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	if (tc == _T(''))
	{
		szTemp.Format(_T("%02d%02d%02d"),
			m_curTime.GetHour(),
			m_curTime.GetMinute(),
			m_curTime.GetSecond());
	}
	else
	{
		szTemp.Format(_T("%02d%c%02d%c%02d"),
			m_curTime.GetHour(), tc,
			m_curTime.GetMinute(), tc,
			m_curTime.GetSecond());
	}
	return szTemp;
}

CString CTimeManager::GetCurDateString(TCHAR tc /*= _T(':')*/)
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	if (tc == _T(''))
	{
		szTemp.Format(_T("%04d%02d%02d"),
			m_curTime.GetYear(), 
			m_curTime.GetMonth(), 
			m_curTime.GetDay());
	}
	else
	{
		szTemp.Format(_T("%04d%c%02d%c%02d"),
			m_curTime.GetYear(), tc,
			m_curTime.GetMonth(), tc,
			m_curTime.GetDay());
	}
	return szTemp;
}

CString CTimeManager::GetCurDayOfWeekString()
{
	enum 
	{
		Sunday = 1,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
	};
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();

	switch(m_curTime.GetDayOfWeek())
	{
	case Sunday:
		szTemp.Format(_T("%s"), _T("Sunday"));
		break;
	case Monday:
		szTemp.Format(_T("%s"), _T("Monday"));
		break;
	case Tuesday:
		szTemp.Format(_T("%s"), _T("Tuesday"));
		break;
	case Wednesday:
		szTemp.Format(_T("%s"), _T("Wednesday"));
		break;
	case Friday:
		szTemp.Format(_T("%s"), _T("Friday"));
		break;
	case Saturday:
		szTemp.Format(_T("%s"), _T("Saturday"));
		break;
	case Thursday:
		szTemp.Format(_T("%s"), _T("Thursday"));
		break;
	}
	
	return szTemp;
}

CString CTimeManager::GetCurSecondString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetSecond());
	return szTemp;
}

int CTimeManager::GetCurSecondInt()
{
	m_curTime = CTime::GetCurrentTime();
	return m_curTime.GetSecond();
}

CString CTimeManager::GetCurMinuteString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetMinute());
	return szTemp;
}

int CTimeManager::GetCurMinuteInt()
{
	m_curTime = CTime::GetCurrentTime();
	return m_curTime.GetMinute();
}

CString CTimeManager::GetCurHourString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetHour());
	return szTemp;
}

int CTimeManager::GetCurHourInt()
{
	m_curTime = CTime::GetCurrentTime();
	return m_curTime.GetHour();
}

CString CTimeManager::GetCurDayString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetDay());
	return szTemp;
}

int CTimeManager::GetCurDayInt()
{
	m_curTime = CTime::GetCurrentTime();
	return m_curTime.GetDay();
}

CString CTimeManager::GetCurMonthString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetMonth());
	return szTemp;
}

int CTimeManager::GetCurMonthInt()
{
	m_curTime = CTime::GetCurrentTime();
	return m_curTime.GetMonth();
}

CString CTimeManager::GetCurYearString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetYear());
	return szTemp;
}

int CTimeManager::GetCurYearInt()
{
	m_curTime = CTime::GetCurrentTime();
	return m_curTime.GetYear();
}

int CTimeManager::GetDaysOfMonth(int month, int year /* = 0 */)
{
	if (!year)	year = GetCurYearInt();
	switch(month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 2:
		if (IsLeapYear(year))
			return 29;
		else
			return 28;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	default:
		return -1;
	}
}

int CTimeManager::GetCurDayIndexOfYear()
{
	int curMonth = GetCurMonthInt();
	int sumDays = GetCurDayInt();
	for (int m = 1; m < curMonth; m++)
	{
		sumDays += GetDaysOfMonth(m);
	}
	return sumDays;
}

int CTimeManager::GetDayIndexOfYear(CString date)
{
	CString szYear = date.Left(4);
	CString szMonth;
	if (isdigit(date.GetAt(4)))
	{
		szMonth = date.Left(6);
		szMonth = szMonth.Right(2);
	}
	else
	{
		szMonth = date.Left(7);
		szMonth = szMonth.Right(2);
	}
	CString szDay = date.Right(2);

	int year = CStringToDec(szYear);
	int month = CStringToDec(szMonth);
	int sumDays = CStringToDec(szDay);
	for (int m = 1; m < month; m++)
	{
		sumDays += GetDaysOfMonth(m, year);
	}
	return sumDays;
}

CString CTimeManager::GetDateByIndexOfYear(int index, int year /* = 0 */, TCHAR tc /* = _T */)
{
	if (!year)	year = GetCurYearInt();
	int day = 0;
 	int m = 1;
	if(index > 0)	// after today
	{
		m = 1;
		while(m)
		{
			if (m > 12) 
			{
				year += m/12;
				m %= 12;
			}
			int daysOfMonth = GetDaysOfMonth(m, year);
			if (index > daysOfMonth)
			{
				m++;
				index -= daysOfMonth;
			}
			else
				break;
		}
		day = index;
	}
	
	if (m == 1 && day == 0)
	{
		year -= 1;
		m = 12;
		day = 31;
	}

	CString szDate;
	if (tc == _T(''))
		szDate.Format(_T("%04d%02d%02d"), year, m, day);
	else
		szDate.Format(_T("%04d%c%02d%c%02d"), year, tc, m, tc, day);
	return szDate;
}

int CTimeManager::GetTotalDaysOfYear(int year)
{
	if (IsLeapYear(year)) 
		return 366;
	else
		return 365;
}

CString CTimeManager::GetDateBeforeToday(int days, TCHAR tc /* = _T */)
{
	int curDayIndex = GetCurDayIndexOfYear();
	int year = GetCurYearInt();
	int totalDays = GetTotalDaysOfYear(year);
	while (days > curDayIndex)
	{
		year -= 1;
		totalDays = GetTotalDaysOfYear(year);
		days -= totalDays;
	}

	int dayIndex = curDayIndex - days;
	return GetDateByIndexOfYear(dayIndex, year, tc);

}

CString CTimeManager::GetDateAfterToday(int days, TCHAR tc /* = _T */)
{
	int dayIndex = GetCurDayIndexOfYear() + days;
	return GetDateByIndexOfYear(dayIndex, GetCurYearInt(), tc);
}

BOOL CTimeManager::IsLeapYear(int year /* = 0 */)
{
	if (!year)	year = GetCurYearInt();

	if ((year%4 == 0 && year%100 != 0)||
		(year%400 == 0))
		return TRUE;
	else
		return FALSE;

}

int CTimeManager::CStringToDec(CString src)
{
	// Generic	SBCS/ANSI	UNICODE
	// _ttoi	atoi		_wtoi
	//	return _ttoi(src);	// method 1
	USES_CONVERSION;
	return strtol(T2A(src), NULL, 10); // method 2
}

//----------------------精确获得算法处理时间的类(毫秒量级)--------------------------//
// 计时开始
void CTimeManager::Start(void) 
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_oldTime);
	m_useTime = 0;
}

// 计时结束
void CTimeManager::End(void)
{
	QueryPerformanceCounter(&m_newTime);
	m_useTime = (double)(m_newTime.QuadPart - m_oldTime.QuadPart) / (double)m_frequency.QuadPart * 1000;
}

// 获得算法处理时间 (单位: ms)
double CTimeManager::GetUseTime(void) 
{
	return m_useTime;
}