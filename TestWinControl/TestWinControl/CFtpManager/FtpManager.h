#pragma once

#include <afxinet.h>
#include <vector>
#include <shlwapi.h>	// for PathFileExisted()
#pragma comment(lib, "shlwapi.lib")
using namespace std;

typedef vector<CString> VString;
typedef vector<CString>::const_iterator c_itVString;
typedef vector<CString>::iterator itVString;

enum FILE_TYPE
{
	TYPE_FILE = 0,
	TYPE_DIR,
	TYPE_ALL,
};

enum FTP_ACTION
{
	ACTION_UPLOAD = 0,
	ACTION_DOWNLOAD,
};

class CFtpManager
{
public:
	CFtpManager();
	~CFtpManager();

private:
	CFtpConnection*	  m_pFtpConn;		// FTP 连接对象
	CInternetSession* m_pInterSession;	// FTP 连接会话
	CString			  m_serverIP;
	CString			  m_username;
	CString			  m_password;
	UINT			  m_connPort;

public:
	BOOL			  m_ftpOnline;
	CString			  m_rootDir;
	CString			  m_localDir;
	CString			  m_remoteDir;
	CString			  m_remoteFile;
	BOOL			  m_remoteIsDir;
	BOOL			  m_bLastDownloadRet;
	BOOL			  m_bLastUploadRet;
	BOOL		      m_curAction;	

public:
	
	// attibute: get and set
	CString GetServerIP() const { return m_serverIP; }
	CString GetUserName() const { return m_username; }
	CString GetPassword() const { return m_password; }
	UINT	GetConnPort() const { return m_connPort; }

	void	SetServerIp(CString serverIP) { m_serverIP = serverIP; }
	void	SetUserName(CString username) { m_username = m_username; }
	void	SetPassword(CString password) { m_password = password; }
	void	SetConnPort(UINT connPort)    { m_connPort = connPort; }

	BOOL    ConnectFtp();
	BOOL    ConnectFtp(CString serverIP, CString username, CString password, UINT connPort);
	BOOL    DisConnectFtp();

	BOOL    DownloadAllInDir(CString remoteDir, CString localDir);	
	BOOL    DownloadSingleFile(CString remoteFile, CString localFile, 
							BOOL bFailedIfExists = FALSE, 
							DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL, 
							DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, 
							DWORD_PTR dwContext = 1);

	BOOL	UploadAllInDir(CString remoteDir, CString localDir);
	BOOL	UploadSingleFile(CString remoteFile, CString localFile,
						  DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, 
						  DWORD_PTR dwContext = 1);

	// only administator has high level rights
	BOOL	CreateRemoteFileOrDir(CString remoteDir, int type = TYPE_DIR);
	BOOL	CreateLocalFileOrDir(CString strDir, int type = TYPE_DIR);

	BOOL	DeleteRemoteFile(CString remoteFile);
	BOOL	DeleteRemoteDir(CString remoteDir);
	BOOL	DeleteLocalFile(CString localFile);
	BOOL	DeleteLocalDir(CString localDir);

	BOOL	RenameRemoteFile(CString oldName, CString newName);
	BOOL	RenameLocalFile(CString oldName, CString newName);

	BOOL	IsRemoteDirExisted(CString remoteDir);
	BOOL	IsLocalDirExisted(CString localDir);
	BOOL	IsRemoteFileExisted(CString remoteFile);
	BOOL	IsLocalFileExisted(CString localFile);

	// Returns TRUE if pszPath is an empty directory. 
	// Returns FALSE if pszPath is not a directory, or if it contains at least one file other than "." or "..".
	BOOL	IsRemoteDirEmpty(CString remoteDir);
	BOOL	IsLocalDirEmpty(CString localDir);		// win api has already provide function "PathIsDirectoryEmpty()" for this	

	BOOL	GetRemoteFileList(CString remoteDir, VString& vTemp, int type = TYPE_FILE);
	BOOL	GetLocalFileList(CString localDir, VString& vTemp, int type = TYPE_FILE);

	BOOL	FindRemoteFilePath(CString remoteDir, VString& vTemp, int type = TYPE_ALL, CString extension = _T("*.*"));
	BOOL	FindLocalFilePath(CString remoteDir, VString& vTemp, int type = TYPE_ALL, CString extension = _T("*.*"));
	BOOL	FindRemoteFileName(CString remoteDir, VString& vTemp, int type = TYPE_ALL, CString extension = _T("*.*"));
	BOOL	FindLocalFileName(CString remoteDir, VString& vTemp, int type = TYPE_ALL, CString extension = _T("*.*"));

	void	ChangeUser(CString user, CString password);
	int		CountChar(CString szTemp,  TCHAR charTemp);	// count char 'c' amount in "szTemp"


	CInternetFile* OpenRemoteFile(LPCTSTR remoteFile, DWORD dwAccess = GENERIC_READ,
								DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY,
								DWORD_PTR dwContext = 1);

protected:
// 	void	ReplaceRightSlashToLeft(CString& szTemp);
// 	void	ReplaceLeftSlashToRight(CString& szTemp);	
// 	void	AddRightSlash(CString& szTemp);
// 	void	AddLeftSlash(CString& szTemp);
// 	void	MinusRightSlash(CString& szTemp);
// 	void	MinusLeftSlash(CString& szTemp);
	void ReplaceRightSlashToLeft(CString& szTemp)  
	{ 
		szTemp.Replace(_T('\\'), _T('/')); 
	}
	void ReplaceLeftSlashToRight(CString& szTemp)	
	{ 
		szTemp.Replace(_T('/'), _T('\\')); 
	}
	void AddRightSlash(CString& szTemp)
	{// if there is no right slash, add it; otherwise don't
		if (szTemp.Right(1) != _T('\\')) szTemp += _T('\\');
	}
	void AddLeftSlash(CString& szTemp)
	{// if there is no left slash, add it; otherwise don't
		if (szTemp.Right(1) != _T('/')) szTemp += _T('/');
	}
	void MinusRightSlash(CString& szTemp)
	{// if there is right slash in the end, minus it; otherwise don't
		if (szTemp.Right(1) == _T('\\'))	szTemp = szTemp.Left(szTemp.GetLength() - 1);
	}
	void MinusLeftSlash(CString& szTemp)
	{// if there is left slash in the end, minus it; otherwise don't
		if (szTemp.Right(1) == _T('/'))	 szTemp = szTemp.Left(szTemp.GetLength() - 1);
	}
};

