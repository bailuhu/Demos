// ExListBox.cpp : implementation file
//

#include "stdafx.h"
#include "VLCDemo.h"
#include "ExListBox.h"


// ExListBox

IMPLEMENT_DYNAMIC(ExListBox, CListBox)

ExListBox::ExListBox()
{
	m_nMaxWidth = 0;
}

ExListBox::~ExListBox()
{
}


BEGIN_MESSAGE_MAP(ExListBox, CListBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// ExListBox message handlers
/********************************************************************/
/*                                                                  */
/* Function name : DrawItem                                         */
/* Description   : Called by the framework when a visual aspect of  */
/*                 an owner-draw list box changes.                  */
/*                                                                  */
/********************************************************************/
void ExListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CListBox::DrawItem(lpDrawItemStruct);
}


void ExListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your code to determine the size of specified item  
	CListBox::MeasureItem(lpMeasureItemStruct);
}

int ExListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	CListBox::CompareItem(lpCompareItemStruct);
	return 0;
}

int ExListBox::AddString(LPCTSTR lpszItem)
{
	int nRet = CListBox::AddString(lpszItem);

	//�˴������ַ�����ȸ��١�ˮƽ��������ʾ�ȴ���  
	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(SB_HORZ, &scrollInfo, SIF_ALL);

	//nPage�������б��ÿҳ���ܹ���ʾ����Ŀ����nMax���б������Ŀ��������nMax���ڻ����nPage���ͳ����˴�ֱ��������
	int nScrollWidth = 0;
	if (GetCount() > 1 && ((int)scrollInfo.nMax >= (int)scrollInfo.nPage))
	{
		nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
	}
	SIZE sSize;
	CClientDC clientDC(this);
	CFont* pListBoxFont = GetFont();

	if (pListBoxFont != NULL)
	{
		CFont* pOldFont = clientDC.SelectObject(pListBoxFont);
		GetTextExtentPoint32(clientDC.m_hDC, lpszItem, strlen(CW2A(lpszItem)), &sSize);
		m_nMaxWidth = max(m_nMaxWidth, (int)sSize.cx);
		SetHorizontalExtent(m_nMaxWidth + 3); //����ˮƽ�������Ĵ�С���ı������һ��СС�Ŀհף����Ĵ�СΪ3
		clientDC.SelectObject(pOldFont);
	}
	return nRet;
}

int ExListBox::DeleteString(UINT nIndex)
{
	int nRet = CListBox::DeleteString(nIndex);

	RECT lRect;
	GetWindowRect(&lRect);
	m_nMaxWidth = lRect.right - lRect.left;

	CClientDC myDC(this);

// 	int i;
// 
// 	if (i < m_nMaxWidth) // ��ʾˮƽ������
// 	{
// 		ShowScrollBar(SB_HORZ, TRUE);
// 		SetHorizontalExtent(m_nMaxWidth);
// 	}
// 	else
// 	{
// 		ShowScrollBar(SB_HORZ, FALSE);
// 	}
	return nRet;
}

void ExListBox::ResetContent()
{
	CListBox::ResetContent();

	m_nMaxWidth = 0;
	SetHorizontalExtent(0);
}