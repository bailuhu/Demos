
// OpenGLDlgDlg.h : ͷ�ļ�
//

#pragma once


// COpenGLDlgDlg �Ի���
class COpenGLDlgDlg : public CDialogEx
{
// ����
public:
	COpenGLDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OPENGLDLG_DIALOG };

protected:
	// ���ɵ���Ϣӳ�亯��
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

// ʵ��
protected:
	HICON m_hIcon;


private:

	int m_nGLPixelIndex;
	HGLRC m_hGLContext;
	BOOL SetWindowPixelFormat(HDC hdc);
	BOOL CreateGLContext(HDC hdc);
	BOOL m_bGLInitialized;

};
