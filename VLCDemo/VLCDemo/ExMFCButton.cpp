// ExMFCButton.cpp : 实现文件
//

#include "stdafx.h"

#include "ExMFCButton.h"

// ExMFCButton

IMPLEMENT_DYNAMIC(ExMFCButton, CMFCButton)

ExMFCButton::ExMFCButton()
{
}

ExMFCButton::~ExMFCButton()
{
}

BEGIN_MESSAGE_MAP(ExMFCButton, CMFCButton)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ExMFCButton 消息处理程序
void ExMFCButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ENSURE(lpDIS != NULL);
	ENSURE(lpDIS->CtlType == ODT_BUTTON);

	CDC* pDCPaint = CDC::FromHandle(lpDIS->hDC);
	ASSERT_VALID(pDCPaint);

	CMemDC memDC(*pDCPaint, this);
	CDC* pDC = &memDC.GetDC();

	CRect rectClient = lpDIS->rcItem;

	OnFillBackground(pDC, rectClient);
	OnDrawBorder(pDC, rectClient, lpDIS->itemState);
	
	// Draw button content:
	COLORREF oldTxtClr = afxGlobalData.clrBtnText;	// save CMFCButton text color
	afxGlobalData.clrBtnText = COLOR_BLACK;			// set ExMFCButton text color
	OnDraw(pDC, rectClient, lpDIS->itemState);
	afxGlobalData.clrBtnText = oldTxtClr;			// restore CMFCButton text color

	if ((lpDIS->itemState & ODS_FOCUS) && m_bDrawFocus)
	{
		OnDrawFocusRect(pDC, rectClient);
	}

}

void ExMFCButton::DrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
	COLORREF bkClr;
	CPen pen(PS_SOLID, 1, COLOR_WHITE);

	if (uiState & ODS_DISABLED) // 按钮不可用状态
		bkClr = COLOR_GREY;
	else
		bkClr = COLOR_LIGHT_BLUE;	//  COLOR_PALE_TURQUOISE

	if (m_bHighlighted)
		bkClr = COLOR_LIGHT_BLUE3;	//  COLOR_MEDIUM_TURQUOISE
	
	if (m_bChecked)
		bkClr = COLOR_LIGHT_BLUE4;	//  COLOR_DARK_TURQUOISE

	CBrush brush(bkClr);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(pen);
	pDC->SelectObject(brush);
	pDC->Rectangle(rectClient);
	if (m_bChecked)
		pDC->Draw3dRect(rectClient, afxGlobalData.clrBtnLight, afxGlobalData.clrBtnHilite);
}

void ExMFCButton::OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
	// TODO: 在此添加专用代码和/或调用基类
	DrawBorder(pDC, rectClient, uiState);
}

