#pragma once

#include <afx.h>	// for CTime
#include <atlstr.h>	// for declaration of CString and LPCTSTR

class CTimeManager : public CTime
{
public:
	CTimeManager(void);
	~CTimeManager(void);

private:
	CTime   m_curTime;

public:
	CString GetCurSecondString();	// 获取当前秒针数，以字符串方式返回
	CString GetCurMinuteString();	// 获取当前分钟数，以字符串方式返回
	CString GetCurHourString();		// 获取当前小时数，以字符串方式返回
	CString GetCurDayString();		// 获取当前日期数，以字符串方式返回
	CString GetCurMonthString();	// 获取当前月份数，以字符串方式返回
	CString GetCurYearString();		// 获取当前年份数，以字符串方式返回
	CString GetCurDayOfWeekString();// 获取当前为一周第几天，以整数方式返回
	CString GetCurDateString(TCHAR tc = _T(':'));// 获取当前年月日，返回：间隔符为 tc 的字符串
	CString GetCurTimeString(TCHAR tc = _T(':'));// 获取当前时分秒，返回：间隔符为 tc 的字符串
	CString GetCurDateAndTimeString(TCHAR tc = _T(':'));// 获取当前年月日时分秒，返回：间隔符为 tc 的字符串

	int		GetCurSecondInt();	// 获取当前秒针数，以整数方式返回
	int		GetCurMinuteInt();	// 获取当前分钟数，以整数方式返回
	int		GetCurHourInt();	// 获取当前小时数，以整数方式返回
	int		GetCurDayInt();		// 获取当前日期数，以整数方式返回
	int		GetCurMonthInt();	// 获取当前月份数，以整数方式返回
	int		GetCurYearInt();	// 获取当前年份数，以整数方式返回
	int		GetCurDayOfWeekInt();// 获取当前为一周第几天，以整数方式返回

	int		GetDaysOfMonth(int month, int year = 0); // 获取某年某月的总天数
	int		GetCurDayIndexOfYear();				// 获取当前日期为今年的第几天
	int		GetDayIndexOfYear(CString date);	// 20141202 or 2014:12:02
	int		GetTotalDaysOfYear(int year);		// 获取某年的总天数
	CString GetDateByIndexOfYear(int index, int year = 0, TCHAR tc = _T(':'));// 获取某年第几天的年月日，返回：间隔符为 tc 的字符串
	CString	GetDateBeforeToday(int days, TCHAR tc = _T(':'));	// 获取今天以前 days 天的年月日，返回：间隔符为 tc 的字符串
	CString	GetDateAfterToday(int days, TCHAR tc = _T(':'));	// 获取今天以后 days 天的年月日，返回：间隔符为 tc 的字符串
	BOOL	IsLeapYear(int year = 0);	// 判断某年是否闰年

private:
	int		CStringToDec(CString src);


// 精确获得算法处理时间的类(毫秒量级)
public:
	void Start(void);	// 计时开始
	void End(void);		// 计时结束
	double GetUseTime(void);// 获得算法处理时间(单位:秒)


private:
	double		  m_useTime;	// 算法处理时间 (单位: ms)
	LARGE_INTEGER m_newTime;
	LARGE_INTEGER m_oldTime;
	LARGE_INTEGER m_frequency;	// 计数值



};

