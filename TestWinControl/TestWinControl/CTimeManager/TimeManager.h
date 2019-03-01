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
	CString GetCurSecondString();	// ��ȡ��ǰ�����������ַ�����ʽ����
	CString GetCurMinuteString();	// ��ȡ��ǰ�����������ַ�����ʽ����
	CString GetCurHourString();		// ��ȡ��ǰСʱ�������ַ�����ʽ����
	CString GetCurDayString();		// ��ȡ��ǰ�����������ַ�����ʽ����
	CString GetCurMonthString();	// ��ȡ��ǰ�·��������ַ�����ʽ����
	CString GetCurYearString();		// ��ȡ��ǰ����������ַ�����ʽ����
	CString GetCurDayOfWeekString();// ��ȡ��ǰΪһ�ܵڼ��죬��������ʽ����
	CString GetCurDateString(TCHAR tc = _T(':'));// ��ȡ��ǰ�����գ����أ������Ϊ tc ���ַ���
	CString GetCurTimeString(TCHAR tc = _T(':'));// ��ȡ��ǰʱ���룬���أ������Ϊ tc ���ַ���
	CString GetCurDateAndTimeString(TCHAR tc = _T(':'));// ��ȡ��ǰ������ʱ���룬���أ������Ϊ tc ���ַ���

	int		GetCurSecondInt();	// ��ȡ��ǰ����������������ʽ����
	int		GetCurMinuteInt();	// ��ȡ��ǰ����������������ʽ����
	int		GetCurHourInt();	// ��ȡ��ǰСʱ������������ʽ����
	int		GetCurDayInt();		// ��ȡ��ǰ����������������ʽ����
	int		GetCurMonthInt();	// ��ȡ��ǰ�·�������������ʽ����
	int		GetCurYearInt();	// ��ȡ��ǰ���������������ʽ����
	int		GetCurDayOfWeekInt();// ��ȡ��ǰΪһ�ܵڼ��죬��������ʽ����

	int		GetDaysOfMonth(int month, int year = 0); // ��ȡĳ��ĳ�µ�������
	int		GetCurDayIndexOfYear();				// ��ȡ��ǰ����Ϊ����ĵڼ���
	int		GetDayIndexOfYear(CString date);	// 20141202 or 2014:12:02
	int		GetTotalDaysOfYear(int year);		// ��ȡĳ���������
	CString GetDateByIndexOfYear(int index, int year = 0, TCHAR tc = _T(':'));// ��ȡĳ��ڼ���������գ����أ������Ϊ tc ���ַ���
	CString	GetDateBeforeToday(int days, TCHAR tc = _T(':'));	// ��ȡ������ǰ days ��������գ����أ������Ϊ tc ���ַ���
	CString	GetDateAfterToday(int days, TCHAR tc = _T(':'));	// ��ȡ�����Ժ� days ��������գ����أ������Ϊ tc ���ַ���
	BOOL	IsLeapYear(int year = 0);	// �ж�ĳ���Ƿ�����

private:
	int		CStringToDec(CString src);


// ��ȷ����㷨����ʱ�����(��������)
public:
	void Start(void);	// ��ʱ��ʼ
	void End(void);		// ��ʱ����
	double GetUseTime(void);// ����㷨����ʱ��(��λ:��)


private:
	double		  m_useTime;	// �㷨����ʱ�� (��λ: ms)
	LARGE_INTEGER m_newTime;
	LARGE_INTEGER m_oldTime;
	LARGE_INTEGER m_frequency;	// ����ֵ



};

