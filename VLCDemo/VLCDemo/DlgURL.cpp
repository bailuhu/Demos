// DlgURL.cpp : implementation file
//

#include "stdafx.h"
#include "VLCDemo.h"
#include "DlgURL.h"
#include "afxdialogex.h"


// DlgURL dialog

IMPLEMENT_DYNAMIC(DlgURL, CDialogEx)

DlgURL::DlgURL(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgURL::IDD, pParent)
{

}

DlgURL::~DlgURL()
{
}

void DlgURL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_URL, m_editURL);
}


BEGIN_MESSAGE_MAP(DlgURL, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgURL::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgURL message handlers


void DlgURL::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_editURL.GetWindowText(m_szUrl, MAX_PATH);
	UpdateData(FALSE);
	CDialogEx::OnOK();
}
