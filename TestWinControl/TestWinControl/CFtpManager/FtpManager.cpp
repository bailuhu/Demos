#include "StdAfx.h"
#include "FtpManager.h"
#include <algorithm>	// for count_if

CFtpManager::CFtpManager(void)
{
	m_ftpOnline = FALSE;
	m_rootDir = _T("Standard/");
	m_remoteIsDir = TRUE;
	m_remoteDir = _T("");
	m_remoteFile = _T("");
	m_localDir = _T("");

	m_serverIP = _T("");
	m_username = _T("");
	m_password = _T("");
	m_connPort	   = 0;
	m_pFtpConn = NULL;
	m_pInterSession = NULL;
}


CFtpManager::~CFtpManager(void)
{
	delete m_pInterSession;
	delete m_pFtpConn;
}

BOOL CFtpManager::ConnectFtp(CString serverIP, CString username, CString password, UINT connPort)
{
	m_serverIP = serverIP;
	m_username = username;
	m_password = password;
	m_connPort = connPort;
	 
	return ConnectFtp();
}

BOOL CFtpManager::ConnectFtp()
{
	m_pInterSession = new CInternetSession;
	m_pInterSession->SetOption( INTERNET_OPTION_CONNECT_TIMEOUT, 500*2);
	m_pInterSession->SetOption( INTERNET_OPTION_CONNECT_RETRIES, 1);
	m_pInterSession->SetOption( INTERNET_OPTION_CONNECT_BACKOFF, 500);

	TRY 
	{	
		m_pFtpConn = m_pInterSession->GetFtpConnection( 
														m_serverIP,
														m_username, 
														m_password,
														m_connPort);	

		m_ftpOnline = TRUE;
	}
	CATCH (CException, e)
	{
		TCHAR error[100];
		memset(error, 0, sizeof(TCHAR)*100);
		e->GetErrorMessage(error, 100);
// 		CString str;
// 		str.Format(_T("%s"), error);
// 		AfxMessageBox(str);

		m_ftpOnline = FALSE;

		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CFtpManager::DisConnectFtp()
{
	m_pFtpConn->Close();
	return TRUE;
}

BOOL CFtpManager::DownloadAllInDir(CString remoteDir, CString localDir)
{
	if (remoteDir.IsEmpty()) return FALSE;
	if (localDir.IsEmpty()) return FALSE;

	ReplaceRightSlashToLeft(localDir);
	ReplaceRightSlashToLeft(remoteDir);

	AddLeftSlash(remoteDir);
	AddLeftSlash(localDir);

	CreateLocalFileOrDir(localDir);

	BOOL bDir = FALSE;
	VString vDirList;
	CString fileName, filePath;

	CFtpFileFind finder(m_pFtpConn);
	BOOL bw = finder.FindFile(remoteDir + _T('*'));
	while(bw)
	{
		bw = finder.FindNextFile();
		if(finder.IsDots()) continue;
		fileName = finder.GetFileName();
		filePath = finder.GetFilePath();


		if (finder.IsDirectory())
		{ 
			bDir = TRUE;
			vDirList.push_back(fileName);
		}
		else	// ִ������
			m_pFtpConn->GetFile(filePath, localDir + fileName, FALSE);
	}

	finder.Close();

	for(c_itVString it = vDirList.begin(); it != vDirList.end(); ++it)
	{
		fileName = *it;
		DownloadAllInDir(remoteDir + fileName, localDir + fileName);
	}

	vDirList.clear();
	return TRUE;
}

BOOL CFtpManager::DownloadSingleFile(CString remoteFile, CString localFile, BOOL bFailedIfExists/* = FALSE*/, 
	DWORD dwAttributes/* = FILE_ATTRIBUTE_NORMAL*/, 
	DWORD dwFlags/* = FTP_TRANSFER_TYPE_BINARY*/, 
	DWORD_PTR dwContext/* = 1*/)
{
	if (remoteFile.IsEmpty()) return FALSE;
	if (localFile.IsEmpty()) return FALSE;

	ReplaceRightSlashToLeft(localFile);
	ReplaceRightSlashToLeft(remoteFile);

	m_pFtpConn->GetFile(remoteFile, localFile, bFailedIfExists, dwAttributes, dwFlags, dwContext);
	return TRUE;
}

BOOL CFtpManager::UploadSingleFile(CString remoteFile, CString localFile,
								  DWORD dwFlags/* = FTP_TRANSFER_TYPE_BINARY*/, 
								  DWORD_PTR dwContext/* = 1*/)
{
	if (remoteFile.IsEmpty()) return FALSE;
	if (localFile.IsEmpty()) return FALSE;

	ReplaceRightSlashToLeft(localFile);
	ReplaceRightSlashToLeft(remoteFile);

	ChangeUser(_T("admin"), _T("vvv855#"));	// ����Ա��ݵ�¼
	return m_pFtpConn->PutFile(localFile, remoteFile , dwFlags, dwContext);
}

BOOL CFtpManager::UploadAllInDir(CString remoteDir, CString localDir)
{
	if (remoteDir.IsEmpty()) return FALSE;
	if (localDir.IsEmpty()) return FALSE;

	ReplaceRightSlashToLeft(localDir);
	ReplaceRightSlashToLeft(remoteDir);

	AddLeftSlash(localDir);
	AddLeftSlash(remoteDir);

	CreateRemoteFileOrDir(remoteDir);

	BOOL bDir = FALSE;
	VString vDirList;
	CString fileName, filePath;

	CFileFind finder;
	BOOL bw = finder.FindFile(localDir + _T('*'));
	while(bw)
	{
		bw = finder.FindNextFile();
		if(finder.IsDots()) continue;
		fileName = finder.GetFileName();
		filePath = finder.GetFilePath();

		if (finder.IsDirectory())
		{ 
			bDir = TRUE;
			vDirList.push_back(fileName);
		}
		else	// ִ���ϴ�
			m_pFtpConn->PutFile(filePath, remoteDir + fileName);
	}

	finder.Close();

	for(c_itVString it = vDirList.begin(); it != vDirList.end(); ++it)
	{
		fileName = *it;
		UploadAllInDir(remoteDir + fileName, localDir + fileName);
	}

	vDirList.clear();
	return TRUE;
}

BOOL CFtpManager::CreateRemoteFileOrDir(CString remoteDir, int type /*= TYPE_DIR*/)
{
	if(remoteDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(remoteDir);

	switch(type)
	{
	case TYPE_DIR:
		// ȥ�� '/'
		MinusLeftSlash(remoteDir);
		// Ŀ¼�����򷵻�
		if(IsRemoteDirExisted(remoteDir)) 
			return FALSE;
		// �ݹ鴴��Ŀ¼
		CreateRemoteFileOrDir(remoteDir.Left(remoteDir.ReverseFind('/')));
		ChangeUser(_T("admin"), _T("vvv855#"));
		return m_pFtpConn->CreateDirectory(remoteDir);
	case TYPE_FILE:
		CString tempDir = remoteDir.Left(remoteDir.ReverseFind(_T('/')) + 1);
		CString	tempName = remoteDir.Right(remoteDir.GetLength() - 1 - remoteDir.ReverseFind(_T('/')));
		CreateRemoteFileOrDir(tempDir, TYPE_DIR);
		CreateLocalFileOrDir(_T("D:\\") + tempName, TYPE_FILE);
		UploadSingleFile(remoteDir, _T("D:\\") + tempName);
		DeleteFile(_T("D:\\") + tempName);
		return TRUE;
	}
	return TRUE;
}

BOOL CFtpManager::CreateLocalFileOrDir(CString localDir, int type /*= TYPE_DIR*/) 
{
	if(localDir.IsEmpty()) return FALSE;
	ReplaceLeftSlashToRight(localDir);

	switch(type)
	{
	case TYPE_DIR:
		if(localDir.Right(1) == _T("\\"))
			localDir = localDir.Left(localDir.GetLength()-1); 
		// Ŀ¼�����򷵻�
		if(GetFileAttributes(localDir) != -1) 
			return FALSE;
		// �ݹ鴴��Ŀ¼
		CreateLocalFileOrDir(localDir.Left(localDir.ReverseFind('\\'))); 
		// ʵ�ʴ���Ŀ¼
		return CreateDirectory(localDir, NULL);

	case TYPE_FILE:
		CreateLocalFileOrDir(localDir.Left(localDir.ReverseFind(_T('\\')) + 1), TYPE_DIR);
		HANDLE hfile = CreateFile(localDir, 
					GENERIC_READ|GENERIC_WRITE,			// ָ�����Դ��ļ��ж�д����
					FILE_SHARE_READ|FILE_SHARE_WRITE,	// ָ���ļ��Ĺ���ģʽ
					NULL,								// ����Ĭ�ϵİ�ȫ��
					OPEN_ALWAYS,						// ���ļ��������򴴽���
					FILE_FLAG_WRITE_THROUGH,			// ����ϵͳ�����Ƴٶ��ļ���д����,
					0);
		// ������ʲô��ʽ�����ں˶������Ƕ�ҪCloseHandle��ϵͳ���������Ѿ�����ʹ�ö���
		CloseHandle(hfile);	// ������رվ������һ�δ������ļ������޷�������
		// hfile = NULL; // �ֲ������Զ����٣���������˶�����
		return TRUE;
	}
	return TRUE;
}

BOOL CFtpManager::DeleteRemoteDir(CString remoteDir)
{
	ChangeUser(_T("admin"), _T("vvv855#"));	// ����Ա��ݵ�¼
	if(remoteDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(remoteDir);
	// ȥ�� '/'
	MinusLeftSlash(remoteDir);
	// Ŀ¼�������򷵻�ʧ��
	if (!IsRemoteDirExisted(remoteDir)) return FALSE;

	// dir ��Ϊ�գ��ݹ�ɾ��dir
	VString vFiles, vDirs;
	FindRemoteFilePath(remoteDir, vFiles, TYPE_FILE);
	FindRemoteFilePath(remoteDir, vDirs, TYPE_DIR);
	for (c_itVString i = vFiles.begin(); i != vFiles.end(); ++i)
	{
		DeleteRemoteFile(*i);
	}
	for (c_itVString i = vDirs.begin(); i != vDirs.end(); ++i)
	{
		DeleteRemoteDir(*i);
	}

	// dir ���Ϊ�գ�ɾ��dir
	if (IsRemoteDirEmpty(remoteDir))
		m_pFtpConn->RemoveDirectory(remoteDir);

	return TRUE;
}

BOOL CFtpManager::DeleteRemoteFile(CString remoteFile)
{
	ChangeUser(_T("admin"), _T("vvv855#"));	// ����Ա��ݵ�¼
	if (remoteFile.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(remoteFile);
	return m_pFtpConn->Remove(remoteFile);
}

BOOL CFtpManager::DeleteLocalFile(CString localFile)
{
	if (localFile.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(localFile);
	return DeleteFile(localFile);
}

BOOL CFtpManager::DeleteLocalDir(CString localDir)
{
	if(localDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(localDir);
	// ȥ�� '/'
	MinusLeftSlash(localDir);
	// Ŀ¼�������򷵻�ʧ��
	if (!IsLocalDirExisted(localDir)) return FALSE;

	// dir ��Ϊ�գ��ݹ�ɾ��dir
	VString vFiles, vDirs;
	FindLocalFilePath(localDir, vFiles, TYPE_FILE);
	FindLocalFilePath(localDir, vDirs, TYPE_DIR);
	for (c_itVString i = vFiles.begin(); i != vFiles.end(); ++i)
	{
		DeleteLocalFile(*i);
	}
	for (c_itVString i = vDirs.begin(); i != vDirs.end(); ++i)
	{
		DeleteLocalDir(*i);
	}

	// dir ���Ϊ�գ�ɾ��dir
	if (IsLocalDirEmpty(localDir))
		RemoveDirectory(localDir);

	return TRUE;
}

void CFtpManager::ChangeUser(CString user, CString password)
{
	m_pFtpConn = m_pInterSession->GetFtpConnection(
								m_serverIP,
								user, 
								password,
								m_connPort);	
}

BOOL CFtpManager::IsRemoteDirEmpty(CString remoteDir)
{
	if (remoteDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(remoteDir);

	CFtpFileFind finder(m_pFtpConn);
	BOOL bw = finder.FindFile(remoteDir + _T("/*.*"));
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		// ֻҪ�ҵ��ļ������� false
// 		if (finder.IsDirectory()) return FALSE;
// 		else return FALSE;
		return FALSE;
	}
	return TRUE;
}

BOOL CFtpManager::IsLocalDirEmpty(CString localDir)
{
	if (localDir.IsEmpty()) return FALSE;
	return PathIsDirectoryEmpty(localDir);
}

BOOL CFtpManager::IsRemoteDirExisted(CString remoteDir)
{
	if (remoteDir.IsEmpty()) return FALSE;

	CFtpFileFind finder(m_pFtpConn);
	return finder.FindFile(remoteDir + _T('*'));
}

BOOL CFtpManager::IsLocalDirExisted(CString localDir)
{
	if (localDir.IsEmpty()) return FALSE;

	CFileFind finder;
	BOOL bw = finder.FindFile(localDir + _T('*'));
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (finder.IsDirectory()) return TRUE;
	}
	return FALSE;
}

BOOL CFtpManager::IsRemoteFileExisted(CString remoteFile)
{
	// ....
	return TRUE;
}

BOOL CFtpManager::IsLocalFileExisted(CString localFile)
{
	if (localFile.IsEmpty()) return FALSE;

	return PathFileExists(localFile);
}

// BOOL CFtpManager::GetRemoteFileList(CString remoteDir, VFtpFileStrut& vTemp, int type /*= TYPE_ALL*/)
// {
// 	if (remoteDir.IsEmpty()) return FALSE;
// 	ReplaceRightSlashToLeft(remoteDir);
// 	AddLeftSlash(remoteDir);
// 
// 	CFtpFileFind finder(m_pFtpConn);
// 	Ftp_File_Structure ftpFileStrut;
// 	BOOL bw = finder.FindFile(remoteDir + _T("*"));
// 	while(bw)
// 	{
// 		bw = finder.FindNextFile();
// 		if (finder.IsDots()) continue;
// 		if (finder.IsDirectory()) ftpFileStrut.isDir = TRUE;
// 		else ftpFileStrut.isDir = FALSE;
// 		switch(type)
// 		{
// 		case TYPE_FILE:
// 			if (ftpFileStrut.isDir)  continue;
// 			break;
// 		case TYPE_DIR:
// 			if (!ftpFileStrut.isDir) continue;
// 			break;
// 		default:
// 
// 			break;
// 		}
// 		ftpFileStrut.fileName = finder.GetFileName();
// 		ftpFileStrut.filePath = finder.GetFilePath();
// 		vTemp.push_back(ftpFileStrut);
// 	}
// 	finder.Close();
// 
// 	for (VFFS it = vTemp.begin(); it != vTemp.end(); ++it)
// 	{
// 		if ((*it).isDir)
// 		{
// 			GetRemoteFileList(remoteDir + (*it).fileName, (*it).vFiles, TYPE_DIR);
// 		}
// 	}
// 
// 	return TRUE;
// }

// BOOL CFtpManager::GetRemoteFileList(CString remoteDir, FFT& vTemp)
// {
// 	if (remoteDir.IsEmpty()) return FALSE;
// 	ReplaceRightSlashToLeft(remoteDir);
// 	AddLeftSlash(remoteDir);
// 
// 	CFtpFileFind finder(m_pFtpConn);
// 
// 	vTemp.me.isDir = TRUE;
// 	BOOL bw = finder.FindFile(remoteDir + _T('*'));
// 	if (!bw)
// 	{
// 		vTemp.me.isEmpty = TRUE;
// 	}
// 	else
// 	{
// 		vTemp.me.isEmpty = FALSE;
// 		while(bw)
// 		{
// 			bw = finder.FindNextFile();
// 			CString fileName = finder.GetFileName();
// 			CString filePath = finder.GetFilePath();
// 			vTemp.me.filePath = filePath;
// 			if (finder.IsDots()) continue;
// 			if (finder.IsDirectory())
// 			{
// 				vTemp.vFolders.push_back(fileName);
// 				vTemp.vFolderPaths.push_back(filePath);
// 			}
// 			else
// 			{
// 				vTemp.vDocs.push_back(fileName);
// 				vTemp.vDocPaths.push_back(filePath);
// 			}
// 		}
// 	}
// 	finder.Close();
// 
// 
// 	return TRUE;
// }

BOOL CFtpManager::GetRemoteFileList(CString remoteDir, VString& vTemp, int type /*= TYPE_FILE*/)
{
	if (remoteDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(remoteDir);
	AddLeftSlash(remoteDir);

	CFtpFileFind finder(m_pFtpConn);
	BOOL bw = finder.FindFile(remoteDir + _T("*"));
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		switch(type)
		{
		case TYPE_FILE:
			if (finder.IsDirectory())  continue;
			break;
		case TYPE_DIR:
			if (!finder.IsDirectory()) continue;
			break;
		default:

			break;
		}
		vTemp.push_back(finder.GetFileName());
	}
	finder.Close();
	return TRUE;
}

BOOL CFtpManager::GetLocalFileList(CString localDir, VString& vTemp, int type/* = TYPE_FILE*/)
{
	if (localDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(localDir);
	AddLeftSlash(localDir);

	CFileFind finder;
	BOOL bw = finder.FindFile(localDir + _T("*"));
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		switch(type)
		{
		case TYPE_FILE:
			if (finder.IsDirectory())  continue;
			break;
		case TYPE_DIR:
			if (!finder.IsDirectory()) continue;
			break;
		default:

			break;
		}
		vTemp.push_back(finder.GetFileName());
	}
	finder.Close();
	return TRUE;
}

BOOL CFtpManager::FindRemoteFilePath(CString remoteDir, VString& vTemp, int type /*= TYPE_ALL*/,  CString extension /*= _T("*.*")*/)
{
	if (remoteDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(remoteDir);
	AddLeftSlash(remoteDir);

	CFtpFileFind finder(m_pFtpConn);
	BOOL isDir;
	BOOL bw = finder.FindFile(remoteDir + extension);
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (finder.IsDirectory()) isDir = TRUE;
		else isDir = FALSE;
		switch(type)
		{
		case TYPE_ALL:
			vTemp.push_back(finder.GetFilePath());
			break;
		case TYPE_DIR:
			if (isDir) vTemp.push_back(finder.GetFilePath());
			break;
		case TYPE_FILE:
			if (!isDir) vTemp.push_back(finder.GetFilePath());
			break;
		}
	}
	return TRUE;
}

BOOL CFtpManager::FindLocalFilePath(CString localDir, VString& vTemp, int type /*= TYPE_ALL*/, CString extension /*= _T("*.*")*/)
{
	if (localDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(localDir);
	AddLeftSlash(localDir);

	CFileFind finder;
	BOOL isDir;
	BOOL bw = finder.FindFile(localDir + extension);
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (finder.IsDirectory()) isDir = TRUE;
		else isDir = FALSE;
		switch(type)
		{
		case TYPE_ALL:
			vTemp.push_back(finder.GetFilePath());
			break;
		case TYPE_DIR:
			if (isDir) vTemp.push_back(finder.GetFilePath());
			break;
		case TYPE_FILE:
			if (!isDir) vTemp.push_back(finder.GetFilePath());
			break;
		}
	}
	return TRUE;
}

BOOL CFtpManager::FindRemoteFileName(CString remoteDir, VString& vTemp, int type /*= TYPE_ALL*/,  CString extension /*= _T("*.*")*/)
{
	if (remoteDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(remoteDir);
	AddLeftSlash(remoteDir);

	CFtpFileFind finder(m_pFtpConn);
	BOOL isDir;
	BOOL bw = finder.FindFile(remoteDir + extension);
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (finder.IsDirectory()) isDir = TRUE;
		else isDir = FALSE;
		switch(type)
		{
		case TYPE_ALL:
			vTemp.push_back(finder.GetFileName());
			break;
		case TYPE_DIR:
			if (isDir) vTemp.push_back(finder.GetFileName());
			break;
		case TYPE_FILE:
			if (!isDir) vTemp.push_back(finder.GetFileName());
			break;
		}
	}
	return TRUE;
}

BOOL CFtpManager::FindLocalFileName(CString localDir, VString& vTemp, int type /*= TYPE_ALL*/, CString extension /*= _T("*.*")*/)
{
	if (localDir.IsEmpty()) return FALSE;
	ReplaceRightSlashToLeft(localDir);
	AddLeftSlash(localDir);

	CFileFind finder;
	BOOL isDir;
	BOOL bw = finder.FindFile(localDir + extension);
	while(bw)
	{
		bw = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (finder.IsDirectory()) isDir = TRUE;
		else isDir = FALSE;
		switch(type)
		{
		case TYPE_ALL:
			vTemp.push_back(finder.GetFileName());
			break;
		case TYPE_DIR:
			if (isDir) vTemp.push_back(finder.GetFileName());
			break;
		case TYPE_FILE:
			if (!isDir) vTemp.push_back(finder.GetFileName());
			break;
		}
	}
	return TRUE;
}

BOOL CFtpManager::RenameRemoteFile(CString oldName, CString newName)
{
	if (oldName.IsEmpty()) return FALSE;
	if (newName.IsEmpty()) return FALSE;

	ReplaceRightSlashToLeft(oldName);
	ReplaceRightSlashToLeft(newName);

	MinusLeftSlash(oldName);
	MinusLeftSlash(newName);

	ChangeUser(_T("lixinccm"), _T("vvv855#"));
	return m_pFtpConn->Rename(oldName, newName);

}

BOOL CFtpManager::RenameLocalFile(CString oldName, CString newName)
{
	if (oldName.IsEmpty()) return FALSE;
	if (newName.IsEmpty()) return FALSE;

	ReplaceRightSlashToLeft(oldName);
	ReplaceRightSlashToLeft(newName);

	MinusLeftSlash(oldName);
	MinusLeftSlash(newName);

	CFile::Rename(oldName, newName);
	return TRUE;
}

int CFtpManager::CountChar(CString szTemp,  TCHAR charTemp)
{
	// ���� '\' �ַ�������ͳ���ļ��в㼶
	ReplaceRightSlashToLeft(szTemp);
	return (int)count_if( (LPCTSTR)szTemp, (LPCTSTR)szTemp+szTemp.GetLength(), bind2nd(equal_to<TCHAR>(), charTemp) );
}

CInternetFile* CFtpManager::OpenRemoteFile(LPCTSTR remoteFile, DWORD dwAccess /* = GENERIC_READ */, 
									DWORD dwFlags /* = FTP_TRANSFER_TYPE_BINARY */, 
									DWORD_PTR dwContext /* = 1 */)
{
	return m_pFtpConn->OpenFile(remoteFile, dwAccess, dwFlags, dwContext);
}


