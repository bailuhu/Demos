#include "StdAfx.h"
#include "GlobalSocket.h"

UINT WSzIPToINetIP(wchar_t* SzIP) 
{ 
	char cIP[32] = {0}; 
	UnicodeToAnsi(SzIP, cIP, 32); 
	return inet_addr(cIP); 
} 

UINT WSzIPToINetIP(wstring SzIP) 
{ 
	char cIP[32] = {0}; 
	UnicodeToAnsi((wchar_t*)SzIP.c_str(), cIP, 32); 
	return inet_addr(cIP); 
}

void ErrorLog(int errCode)
{
	switch(errCode)
	{
	case WSAETIMEDOUT:
		break;
	default:
		break;
	}
	//................
}


wstring INetIPtoWSzIP(UINT inetIP) 
{ 
	wchar_t tmp[32] = {0}; 
	unsigned char* cIP = (unsigned char*)&inetIP;
	swprintf_s(tmp, 32, L"%u.%u.%u.%u", *cIP, *(cIP + 1), *(cIP + 2), *(cIP + 3));

	return wstring(tmp); 
} 

string INetIPtoSzIP(UINT inetIP) 
{ 
	char tmp[32] = {0}; 
	unsigned char* cIP = (unsigned char*)&inetIP;
	sprintf_s(tmp, 32, "%u.%u.%u.%u", *cIP, *(cIP + 1), *(cIP + 2), *(cIP + 3));
	return string(tmp); 
} 


bool INetIPtoSzIP(UINT inetIP, char* buf, int size) 
{ 
	if (size < 16) return false; 
	memset(buf, 0, size); 
	unsigned char* cIP = (unsigned char*)&inetIP;
	sprintf_s(buf, 32, "%u.%u.%u.%u", *cIP, *(cIP + 1), *(cIP + 2), *(cIP + 3));
	return true; 
} 

bool INetIPtoWSzIP(UINT inetIP, wchar_t* buf, int size) 
{ 
	if (size < 16) return false; 
	memset(buf, 0, size); 
	unsigned char* cIP = (unsigned char*)&inetIP;
	swprintf_s(buf, 32, L"%u.%u.%u.%u", *cIP, *(cIP + 1), *(cIP + 2), *(cIP + 3)); 
	return true; 
} 

UINT wstingIpToInetIP(wchar_t* SzIP) 
{ 
	char cIP[32] = {0}; 
	UnicodeToAnsi(SzIP, cIP, 32); 
	return inet_addr(cIP); 
} 

UINT wstingIpToInetIP(wstring SzIP) 
{ 
	char cIP[32] = {0}; 
	UnicodeToAnsi((wchar_t*)SzIP.c_str(), cIP, 32); 
	return inet_addr(cIP); 
} 

UINT SzIPToINetIP(char* SzIP) 
{ 
	return inet_addr(SzIP); 
}


SOCKADDR_IN GetSocketAddr(LPSTR Ip, LPSTR Port)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(Ip);	// 将字符串转换为32位二进制网络字节序的IPV4地址，否则为INADDR_NONE
	sockAddr.sin_port = htons(atoi(Port));	// 一个无符号短整型的主机数值转换为网络字节顺序，即大尾顺序(big-endian)

	return sockAddr;
}

SOCKADDR_IN GetSocketAddr(wchar_t * Ip, wchar_t * Port)
{
	char cIP[16] = {0};
	char cPort[6] = {0};
	UnicodeToAnsi(Ip, cIP, 16);
	UnicodeToAnsi(Port, cPort, 6);
	return GetSocketAddr(cIP, cPort);
}

SOCKADDR_IN GetSocketAddr(const wchar_t * Ip, const wchar_t * Port)
{
	char cIP[16] = {0};
	char cPort[6] = {0};
	UnicodeToAnsi((wchar_t*)Ip, cIP, 16);
	UnicodeToAnsi((wchar_t*)Port, cPort, 6);
	return GetSocketAddr(cIP, cPort);
}


SOCKADDR_IN GetSocketAddr(UINT INetIP, USHORT INetPort)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = INetIP;	// 将字符串转换为32位二进制网络字节序的IPV4地址，否则为INADDR_NONE
	sockAddr.sin_port = (USHORT)INetPort;	// 一个无符号短整型的主机数值转换为网络字节顺序，即大尾顺序(big-endian)

	return sockAddr;
}

SOCKADDR_IN GetSocketAddr(USHORT nPort)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);;	// 将字符串转换为32位二进制网络字节序的IPV4地址，否则为INADDR_NONE
	sockAddr.sin_port = htons(nPort);	// 一个无符号短整型的主机数值转换为网络字节顺序，即大尾顺序(big-endian)

	return sockAddr;
}

CString GetHostWSzIP(void)
{
	hostent* host;
	char hostName[64];
	gethostname(hostName,64);
	host = gethostbyname(hostName);

	CString strIPaddr;

	strIPaddr = inet_ntoa(*(in_addr *)host->h_addr_list[0]);

	return strIPaddr;
}

ULONG GetHostNetIP(void)
{
	hostent* host;
	char hostName[64];
	gethostname(hostName,64);
	host = gethostbyname(hostName);

	ULONG IP;

	memcpy(&IP, host->h_addr_list[0], 4);

	return IP;
}


	/*
//	1.设置调用closesocket()后,仍可继续重用该socket。调用closesocket()一般不会立即关闭socket，而经历TIME_WAIT的过程。
	BOOL bReuseaddr = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL) );

//	2. 如果要已经处于连接状态的soket在调用closesocket()后强制关闭，不经历TIME_WAIT的过程：
	BOOL bDontLinger = FALSE;
	setsockopt( sock, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL) );

//	3.在send(),recv()过程中有时由于网络状况等原因，收发不能预期进行,可以设置收发时限：
	int iTimeout = 1000; //1秒
	setsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&iTimeout, sizeof(int) );	// 发送时限
	setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&iTimeout, sizeof(int) );	// 接收时限

//	4.在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节(异步)；系统默认的状态发送和接收一次为8688字节(约
//	为8.5K)；在实际的过程中如果发送或是接收的数据量比较大，可以设置socket缓冲区，避免send(),recv()不断的循环收发：
	int nRecvBufLen = 32 * 1024; //设置为32K
	setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBufLen, sizeof(int) );	// 接收缓冲区
	int nSendBufLen = 32*1024; //设置为32K
	setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBufLen, sizeof(int) );	// 发送缓冲区

//	5.在发送数据的时，不执行由系统缓冲区到socket缓冲区的拷贝，以提高程序的性能：
	int nZero = 0;
	setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (const char *)&nZero, sizeof(nZero) );

//	6.在接收数据时，不执行将socket缓冲区的内容拷贝到系统缓冲区：
	int nZero = 0;
	setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (const char *)&nZero, sizeof(int) );

//	7.一般在发送UDP数据报的时候，希望该socket发送的数据具有广播特性：
	BOOL bBroadcast = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (const char*)&bBroadcast, sizeof(BOOL) );

//	8.在client连接服务器过程中，如果处于非阻塞模式下的socket在connect()的过程中可以设置connect()延时,直到accpet()被调用(此设置只
//	有在非阻塞的过程中有显著的作用，在阻塞的函数调用中作用不大)
	BOOL bConditionalAccept = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (const char*)&bConditionalAccept, sizeof( BOOL ) );

//	9.如果在发送数据的过程中send()没有完成，还有数据没发送，而调用了closesocket(),以前一般采取的措施是shutdown(s,SD_BOTH),但是数
//	据将会丢失。某些具体程序要求待未发送完的数据发送出去后再关闭socket，可通过设置让程序满足要求：
	struct linger {
		u_short l_onoff;
		u_short l_linger;
	};
	linger m_sLinger;
	m_sLinger.l_onoff = 1;  // 在调用closesocket()时还有数据未发送完，允许等待
//若m_sLinger.l_onoff=0; 则调用closesocket()后强制关闭
	m_sLinger.l_linger = 5; // 设置等待时间为5秒
	setsockopt( sock, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(linger) );
	*/

