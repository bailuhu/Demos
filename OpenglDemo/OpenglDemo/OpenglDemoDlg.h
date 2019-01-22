
// OpenGLDlgDlg.h : 头文件
//

#pragma once


// COpenGLDlgDlg 对话框
class COpenGLDlgDlg : public CDialogEx
{
// 构造
public:
	COpenGLDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OPENGLDLG_DIALOG };

protected:
	// 生成的消息映射函数
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

// 实现
protected:
	HICON m_hIcon;


private:

	int m_nGLPixelIndex;
	HGLRC m_hGLContext;
	BOOL SetWindowPixelFormat(HDC hdc);
	BOOL CreateGLContext(HDC hdc);
	BOOL m_bGLInitialized;

};
