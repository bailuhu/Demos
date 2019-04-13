#include "StdAfx.h"

#include "TcpClientManager.h"

TcpClientManager::TcpClientManager(void)
{
	m_Sock = INVALID_SOCKET;
	m_bInitialized = FALSE;
	m_bConnected = FALSE;
}

TcpClientManager::~TcpClientManager(void)
{
	CloseSocket();
}


BOOL TcpClientManager::InitWSA()
{
	enum
	{
		DeputyVerNum = 1,
		MainVerNum = 2,
	};

	WORD wVersionRequired;
	WSADATA wsaData;
	wVersionRequired = MAKEWORD(DeputyVerNum, MainVerNum);

	int nRet = WSAStartup(wVersionRequired, &wsaData);

	if (nRet == 0)
		return TRUE;
	else
		ErrorLog(nRet);

	if (HIBYTE(wsaData.wVersion) != DeputyVerNum ||
		LOBYTE(wsaData.wVersion) != MainVerNum)
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL TcpClientManager::CloseSocket()
{
	if (closesocket(m_Sock) == SOCKET_ERROR)
	{
		ErrorLog(WSAGetLastError());		
	}
	m_bInitialized = FALSE;
	return TRUE;
}

BOOL TcpClientManager::ReleaseWSA()
{
	if (WSACleanup() == SOCKET_ERROR)
	{
		ErrorLog(WSAGetLastError());	
	}

	return TRUE;
}

BOOL TcpClientManager::InitSocket()
{
	// GetTcpSocket
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET)	
	{
		AfxMessageBox(_T("连接服务器套接字创建失败!"));
		return FALSE;
	}

// 	//取消延时
// 	int on = 1;  
// 	setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(on));  

	return TRUE;
}

int TcpClientManager::Init()
{
	m_bInitialized = FALSE;

	if (!InitWSA()) return SE_INIT_FAIL;

	if(!InitSocket())
	{
		Close();
		return SE_INIT_FAIL;
	}

	m_bInitialized = TRUE;
	return SE_OK;
}

void TcpClientManager::Close()
{
	CloseSocket();
	m_bInitialized = FALSE;
}

int TcpClientManager::SetSocketOption(UINT bufsize, UINT SendTimeOut, UINT RecvTimeOut)
{
	if (bufsize > 0)
	{
		setsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(int) );
		setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(int) );
	}

	if (SendTimeOut > 0)	
	{
		setsockopt(m_Sock, SOL_SOCKET, SO_SNDTIMEO,(const char*)&SendTimeOut, sizeof(int) ); 
	}

	if (RecvTimeOut > 0)	
	{
		setsockopt(m_Sock, SOL_SOCKET, SO_RCVTIMEO,(const char*)&RecvTimeOut, sizeof(int) ); 	
	}

	return 0;
}


int TcpClientManager::Connect(UINT INetIP, USHORT INetPort)
{
	SOCKADDR_IN addrClient = GetSocketAddr(INetIP, INetPort);

	int nRet = connect(m_Sock, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));

	if (nRet == SOCKET_ERROR) return -1;
	
	return 0;
}

int TcpClientManager::Connect(wstring IP, USHORT port)
{
	UINT INetIP = wstingIpToInetIP(IP);
	USHORT INetPort = htons(port);

	SOCKADDR_IN addrClient = GetSocketAddr(INetIP, INetPort);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0); 

	if (m_Sock == SOCKET_ERROR)
	{
		return -1;
	}

	int nRet = connect(m_Sock, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));

	if (nRet == SOCKET_ERROR) return -1;

	return 0;
}

int TcpClientManager::Connect(wstring IP, wstring Port)
{
	SOCKADDR_IN addrClient = GetSocketAddr(IP.c_str(), Port.c_str());

	int nRet = connect(m_Sock, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));

	if (nRet == SOCKET_ERROR) return -1;

	return 0;
}

int TcpClientManager::Send(wstring szData)
{
	char* Data = UnicodeToUtf8((wchar_t*)szData.c_str());//转换为utf-8

	int length = (int)strlen(Data);

	int val = send(m_Sock, Data, length, 0);

	delete [] Data;

	if (val != length)		return SE_SEND_FAIL;

	return SE_OK;
}

//int TcpClientManager::Send(string szData)
//{
//
//	int length = szData.length();
//
//	//char* Data = (char*)&szData.c_str()[0];
//	const int bufsize = 1024*64;
//	char Data[bufsize] = {0};
//	szData.copy(Data, length, 0);
//
//	int val = send(m_Sock, Data, length, 0);
//
//
//	if (val != length)		return SE_SEND_FAIL;
//
//	return SE_OK;
//}

int TcpClientManager::Send(char* Data, int length)
{
	int val = send(m_Sock, Data, length, 0);

	if (val != length)		return SE_SEND_FAIL;

	return SE_OK;
}

int TcpClientManager::Recieve(wstring& Data)
{
	const int bufsize = 1024*64; //64K
	char buf[bufsize] = {0};

	int nCnt = recv(m_Sock, buf, bufsize, 0);

	if (nCnt == 0) return SE_SOCKET_DISCONNECT;
	if (nCnt<0)
	{
		return SE_RECV_FAIL;
	}

	wchar_t* p = Utf8ToUnicode(buf);
	Data = p;

	delete [] p;

	return SE_OK;
}



//读取指定的长度
int TcpClientManager::Recieve(char* Data, int len, int& nRecvLen)
{
	int nCnt = recv(m_Sock, Data, len, 0);
	if (nCnt == 0) 
		return SE_SOCKET_DISCONNECT;

	if (nCnt<0)
	{
		return SE_RECV_FAIL;
	}

	nRecvLen = nCnt;

	return SE_OK;
}


int TcpClientManager::SetNonBlockMode(BOOL bNon)
{
	unsigned long ul = 1;
	if (bNon)
		ul = 1; //非阻塞
	else
		ul = 0;

	int nRet = ioctlsocket(m_Sock, FIONBIO, &ul);
	if(nRet == SOCKET_ERROR) 
		return SE_SOCKET_ERROR;
	else
		return SE_OK;
}



int TcpClientManager::Connect_withTimeOut(wstring IP, wstring Port, int wTime)
{
	//切换为非阻塞式
	if (SetNonBlockMode(TRUE) != SE_OK) return SE_FAIL_TO_CONNECT;

	//连接
	Connect(IP, Port);
	//设置超时
	timeval Time;
	Time.tv_sec = wTime/1000; //连接超时
	Time.tv_usec = wTime - 1000*Time.tv_sec;

	fd_set r;
	FD_ZERO(&r);
	FD_SET(m_Sock, &r);
	//判断返回值
	int nRet = select(0, 0, &r, 0, &Time);
	if ( nRet <= 0 )   return SE_FAIL_TO_CONNECT;

	//切换为阻塞式
	if (SetNonBlockMode(FALSE) != SE_OK) return SE_FAIL_TO_CONNECT;

	return SE_OK;
}


int TcpClientManager::SetSendTimeout(SOCKET& Sock, int Timeout)
{
	// 在send(),recv()过程中有时由于网络状况等原因，收发不能预期进行,可以设置收发时限
	// timeout: milisecond
	return setsockopt(Sock, SOL_SOCKET, SO_SNDTIMEO,(const char*)&Timeout, sizeof(int) ); 
}

int TcpClientManager::SetRecvTimeout(SOCKET& Sock, int Timeout)
{
	// 在send(),recv()过程中有时由于网络状况等原因，收发不能预期进行,可以设置收发时限
	// timeout: milisecond
	return setsockopt(Sock, SOL_SOCKET, SO_RCVTIMEO,(const char*)&Timeout, sizeof(int) ); 
}

int TcpClientManager::SetRecBuf(SOCKET& Sock, int bufsize)
{
	return setsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(int) );
}