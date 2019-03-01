#pragma once
enum ALIGNTYPE
{ 
	ALIGN_NONE = 0, //��ͣ�� 
	ALIGN_TOP, //ͣ���ϱ� 	
	ALIGN_RIGHT, //ͣ���ұ� 
	ALIGN_LEFT, //ͣ����� 
	ALIGN_BOTTOM
}; 

// CLessWindowDlg �Ի���

class CLessWindowDlg : public CDialog
{
	DECLARE_DYNAMIC(CLessWindowDlg)

public:
	CLessWindowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLessWindowDlg();

// �Ի�������
	enum { IDD = IDD_LESSWINDOW };
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		void ShowAlphaWindow(ALIGNTYPE m_eAlignType);
		void OnClose();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	IDirectUI *m_pDirectUI; 	
	IUIFormObj* m_pMainBack1;
	int			m_nOp1;

	ALIGNTYPE m_eAlignType; //��¼����ͣ��״̬

	int nxFullScreen;
	int nyFullScreen;
};
