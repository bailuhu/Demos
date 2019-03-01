// MailMessage.h: interface for the CMailMessage class.
// Copyright (c) 1998, Wes Clyburn
//
// Modified to have Header and Body handling in this class rather than in any
// class that uses instances of CMailMessage.
// Copyright (c) 1998 Michael Krebs
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_)
#define AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/************************************************************************/
/* //����
void CMyEmailClientDlg::OnSend() 
{
	//��������
	UpdateData( TRUE );

	//����CSMTP����
	CSMTP smtp( m_SMTP );

	//�½�CMailMessage���󣬲���ʼ��
	CMailMessage msg;
	msg.m_sFrom = m_From;
	msg.AddMultipleRecipients( m_To );
	msg.m_sSubject = m_Subject;
	msg.m_sBody = m_Body;

	//����SMTP������
	if( !smtp.Connect() )
	{
		AfxMessageBox( smtp.GetLastError() );
		return;
	}

	//����
	if( !smtp.SendMessage( &msg ) )
	{
		AfxMessageBox( smtp.GetLastError() );
		return;
	}

	//�Ͽ�����
	if( !smtp.Disconnect() )
	{
		AfxMessageBox( smtp.GetLastError() );
		return;
	}
	AfxMessageBox( _T( "Message Sent Successfully") );
}

//����
void CMyEmailClientDlg::OnRetr() 
{
	//��������
	UpdateData( TRUE );

	//�½�CPOP3����
	CPOP3 pop3( m_POP3 );
	pop3.SetUserProperties(m_User,m_Password);

	//����POP3������
	if (!pop3.Connect())
	{
		AfxMessageBox( pop3.GetLastError() );
		return;
	}

	//��ȡ�ż�
	CMailMessage msg;
	if (!pop3.GetMessage(1,&msg))
	{
		AfxMessageBox( pop3.GetLastError() );
		return;
	}

	//��ʾ�ż�
	m_Body=msg.m_sBody;
	m_Subject=msg.m_sSubject;
	m_From=msg.m_sFrom;
	m_To="";
	for (int a=0; a<msg.GetNumRecipients(); a++)
	{
		CString sEmail;
		CString sFriendly;
		msg.GetRecipient(sEmail,sFriendly,a);
		m_To+=sEmail;
		m_To+=" ";
	}
	m_To.TrimRight();

	//�Ͽ�����
	if( !pop3.Disconnect() )
	{
		AfxMessageBox( pop3.GetLastError() );
		return;
	}
	AfxMessageBox( _T( "Successfully disconnected" ) );

	UpdateData(FALSE);
}
                                                                     */
/************************************************************************/
#include <afxtempl.h>

class CMailMessage  
{
public:
	CMailMessage();
	virtual ~CMailMessage();

	int  GetNumRecipients();
	BOOL GetRecipient(CString& sEmailAddress, CString& sFriendlyName, int nIndex = 0 );
	BOOL AddRecipient(LPCTSTR szEmailAddress, LPCTSTR szFriendlyName = _T("") );
	BOOL AddMultipleRecipients( LPCTSTR szRecipients = NULL );

	BOOL EncodeHeader();// Create the SMTP message header as per RFC822
	BOOL DecodeHeader();// Read fields from Header - NOT COMPLETED

	void EncodeBody(); // Exchange .CR/LF by ..CR/LF
	void DecodeBody(); // There's no Base64/Mime/UU en- or decoding done here !

	CString m_sFrom;
	CString m_sSubject;
	CString m_sHeader;
	CTime m_tDateTime;	
	CString m_sBody;

private:
	class CRecipient
	{
		public:
			CString m_sEmailAddress;
			CString m_sFriendlyName;
	};
	CArray <CRecipient, CRecipient&> m_Recipients;
};

#endif // !defined(AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_)
