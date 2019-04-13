#pragma once

//Coder Nicolas Lee 
//C++ Unicode version.
//BE = Big Endian  LE = Little Endian(Default)
//代码类所有未明确的说明的 无符号32位整形和16位整形 均为网络字节序 2016.2.17

#include "GlobalFunction.h"
#include "StringProc.h"

// if u include windows.h, may be u should use 注释掉 winsock2.h
// if u didn't include windows.h, may be u should use winsock2.h
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


#include <string>
using namespace std;

#pragma pack(push, 1)  //内存中对齐方式(1字节) 需与测试软件定义一致
typedef struct
{
	int  CmdID;		// 指令 1~99为测试项目ID，100~199启动、关闭模组、调焦命令，200~300获取测试项目信息
	int  Status;		// 回应请求：0执行请求，-1不执行请求
	int  Result;	// 回应结果:1 OK, 0 NG, -1 继续
	char ErrInfo[256];  //附加信息字符串
	char TestData[256]; //测试数据
}SockMsg;
#pragma pack(pop)

#define BEAT_TIME 1000

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{	if(p) { delete (p);						(p)=NULL; }	}
#define SAFE_DELETE_DLG(p)		{	if(p) { p->DestroyWindow();delete (p);	(p)=NULL; }	}
#define SAFE_DELETE_ARRAY(p)	{	if(p) { delete [] (p);					(p)=NULL; }	}
#define SAFE_CLOSE_HANDLE(p)	{	if(p) { ::CloseHandle(p);				(p)=NULL; }	}
#define SAFE_DELETE_OBJECT(p)	{	if(p) { ::DeleteObject((HGDIOBJ) p);	(p)=NULL; }	}
#endif	/* SAFE_DELETE */

#ifndef SE_SOCKET
#define SE_OK                 0
#define SE_NOT_INIT          -1  //未初始化
#define SE_SEND_FAIL         -2  //发送失败
#define SE_RECV_FAIL         -3  //接收数据失败
#define SE_INIT_WINSOCK_FAIL -4  //初始化失败
#define SE_FAIL_TO_CONNECT   -5  //连接失败
#define SE_FAIL_TO_ACCEPT    -6  //接收失败
#define SE_ERROR_SOCKET      -7  //接收失败
#define SE_SOCKET_DISCONNECT -8  //连接中断，失去连接
#define SE_TIME_OUT          -9  
#define SE_SOCKET_ERROR      -10  
#define SE_BUFFER_OVERFLOW   -11  
#define SE_UNINITIALIZED	 -12	// 连接初始化失败
#define SE_INIT_SOCKET_FAIL  -13  //初始化失败
#define SE_INIT_FAIL         -14  //初始化失败
#endif

void ErrorLog(int errCode);

wstring INetIPtoWSzIP(UINT inetIP) ;
string INetIPtoSzIP(UINT inetIP) ;
bool INetIPtoSzIP(UINT inetIP, char* buf, int size) ;
bool INetIPtoWSzIP(UINT inetIP, wchar_t* buf, int size) ;
UINT wstingIpToInetIP(wchar_t* SzIP) ;
UINT wstingIpToInetIP(wstring SzIP) ;
UINT SzIPToINetIP(char* SzIP);
SOCKADDR_IN GetSocketAddr(LPSTR Ip, LPSTR Port);
SOCKADDR_IN GetSocketAddr(wchar_t * Ip, wchar_t * Port);
SOCKADDR_IN GetSocketAddr(const wchar_t* Ip, const wchar_t* Port);
SOCKADDR_IN GetSocketAddr(UINT INetIP, USHORT INetPort);
SOCKADDR_IN GetSocketAddr(USHORT nPort);

UINT WSzIPToINetIP(wchar_t* SzIP);

//得到本机IP 字符串
CString GetHostWSzIP(void);
//得到本机IP 整形
ULONG GetHostNetIP(void);