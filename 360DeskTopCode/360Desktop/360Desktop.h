// 360Desktop.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CMy360DesktopApp:
// �йش����ʵ�֣������ 360Desktop.cpp
//

class CMy360DesktopApp : public CWinApp
{
public:
	CMy360DesktopApp();

	// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	IDUIRes* m_pDUIRes;

	// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy360DesktopApp theApp;