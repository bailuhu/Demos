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
	sockAddr.sin_addr.S_un.S_addr = inet_addr(Ip);	// ���ַ���ת��Ϊ32λ�����������ֽ����IPV4��ַ������ΪINADDR_NONE
	sockAddr.sin_port = htons(atoi(Port));	// һ���޷��Ŷ����͵�������ֵת��Ϊ�����ֽ�˳�򣬼���β˳��(big-endian)

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
	sockAddr.sin_addr.S_un.S_addr = INetIP;	// ���ַ���ת��Ϊ32λ�����������ֽ����IPV4��ַ������ΪINADDR_NONE
	sockAddr.sin_port = (USHORT)INetPort;	// һ���޷��Ŷ����͵�������ֵת��Ϊ�����ֽ�˳�򣬼���β˳��(big-endian)

	return sockAddr;
}

SOCKADDR_IN GetSocketAddr(USHORT nPort)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);;	// ���ַ���ת��Ϊ32λ�����������ֽ����IPV4��ַ������ΪINADDR_NONE
	sockAddr.sin_port = htons(nPort);	// һ���޷��Ŷ����͵�������ֵת��Ϊ�����ֽ�˳�򣬼���β˳��(big-endian)

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
//	1.���õ���closesocket()��,�Կɼ������ø�socket������closesocket()һ�㲻�������ر�socket��������TIME_WAIT�Ĺ��̡�
	BOOL bReuseaddr = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL) );

//	2. ���Ҫ�Ѿ���������״̬��soket�ڵ���closesocket()��ǿ�ƹرգ�������TIME_WAIT�Ĺ��̣�
	BOOL bDontLinger = FALSE;
	setsockopt( sock, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL) );

//	3.��send(),recv()��������ʱ��������״����ԭ���շ�����Ԥ�ڽ���,���������շ�ʱ�ޣ�
	int iTimeout = 1000; //1��
	setsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&iTimeout, sizeof(int) );	// ����ʱ��
	setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&iTimeout, sizeof(int) );	// ����ʱ��

//	4.��send()��ʱ�򣬷��ص���ʵ�ʷ��ͳ�ȥ���ֽ�(ͬ��)���͵�socket���������ֽ�(�첽)��ϵͳĬ�ϵ�״̬���ͺͽ���һ��Ϊ8688�ֽ�(Լ
//	Ϊ8.5K)����ʵ�ʵĹ�����������ͻ��ǽ��յ��������Ƚϴ󣬿�������socket������������send(),recv()���ϵ�ѭ���շ���
	int nRecvBufLen = 32 * 1024; //����Ϊ32K
	setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBufLen, sizeof(int) );	// ���ջ�����
	int nSendBufLen = 32*1024; //����Ϊ32K
	setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBufLen, sizeof(int) );	// ���ͻ�����

//	5.�ڷ������ݵ�ʱ����ִ����ϵͳ��������socket�������Ŀ���������߳�������ܣ�
	int nZero = 0;
	setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (const char *)&nZero, sizeof(nZero) );

//	6.�ڽ�������ʱ����ִ�н�socket�����������ݿ�����ϵͳ��������
	int nZero = 0;
	setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (const char *)&nZero, sizeof(int) );

//	7.һ���ڷ���UDP���ݱ���ʱ��ϣ����socket���͵����ݾ��й㲥���ԣ�
	BOOL bBroadcast = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (const char*)&bBroadcast, sizeof(BOOL) );

//	8.��client���ӷ����������У�������ڷ�����ģʽ�µ�socket��connect()�Ĺ����п�������connect()��ʱ,ֱ��accpet()������(������ֻ
//	���ڷ������Ĺ����������������ã��������ĺ������������ò���)
	BOOL bConditionalAccept = TRUE;
	setsockopt( sock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (const char*)&bConditionalAccept, sizeof( BOOL ) );

//	9.����ڷ������ݵĹ�����send()û����ɣ���������û���ͣ���������closesocket(),��ǰһ���ȡ�Ĵ�ʩ��shutdown(s,SD_BOTH),������
//	�ݽ��ᶪʧ��ĳЩ�������Ҫ���δ����������ݷ��ͳ�ȥ���ٹر�socket����ͨ�������ó�������Ҫ��
	struct linger {
		u_short l_onoff;
		u_short l_linger;
	};
	linger m_sLinger;
	m_sLinger.l_onoff = 1;  // �ڵ���closesocket()ʱ��������δ�����꣬����ȴ�
//��m_sLinger.l_onoff=0; �����closesocket()��ǿ�ƹر�
	m_sLinger.l_linger = 5; // ���õȴ�ʱ��Ϊ5��
	setsockopt( sock, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(linger) );
	*/

