#include "StdAfx.h"
#include "TimeManager.h"


TimeManager::TimeManager(void)
{

}


TimeManager::~TimeManager(void)
{
}

CString TimeManager::GetCurrentTimeString(TCHAR tc /*= _T(':')*/)
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d%c%d%c%d%c%d%c%d%c%d"),
		          m_curTime.GetYear(), tc,
				  m_curTime.GetMonth(), tc,
				  m_curTime.GetDay(), tc,
				  m_curTime.GetHour(), tc,
				  m_curTime.GetMinute(), tc,
				  m_curTime.GetSecond());

	return szTemp;
}

CString TimeManager::GetCurDayOfWeekString()
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

CString TimeManager::GetCurSecondString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetSecond());
	return szTemp;
}

CString TimeManager::GetCurMinuteString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetMinute());
	return szTemp;
}

CString TimeManager::GetCurHourString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetHour());
	return szTemp;
}

CString TimeManager::GetCurDayString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetDay());
	return szTemp;
}

CString TimeManager::GetCurMonthString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetMonth());
	return szTemp;
}

CString TimeManager::GetCurYearString()
{
	CString szTemp;
	m_curTime = CTime::GetCurrentTime();
	szTemp.Format(_T("%d"), m_curTime.GetYear());
	return szTemp;
}