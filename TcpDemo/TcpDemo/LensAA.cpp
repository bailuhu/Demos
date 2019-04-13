#include "stdafx.h"
#include "LensAA.h"


LensAA::LensAA()
{
	finsConnected = false;
	m_axis = 8;
}


LensAA::~LensAA()
{

}

int LensAA::OpenFins(wstring ip, int port)
{
	
	if (m_tcpClient.Init() != SE_OK)
	{
		// init client ng
		return SE_INIT_WINSOCK_FAIL;
	}

	if (m_tcpClient.Connect_withTimeOut(ip, to_wstring(port), 1000) != SE_OK)
	{
		return SE_FAIL_TO_CONNECT;
	}

	// hand shake
	//FINS帧格式
	const int sendLen = 20;
	char Handshake[sendLen] = { 0 };
	Handshake[0] = 0x46;        //Head FINS
	Handshake[1] = 0x49;
	Handshake[2] = 0x4e;
	Handshake[3] = 0x53;

	Handshake[4] = 0x00;        //发送的数据长度，从[8]开始算
	Handshake[5] = 0x00;
	Handshake[6] = 0x00;
	Handshake[7] = 0x0c;

	Handshake[8] = 0x00;        //命令，固定为0
	Handshake[9] = 0x00;
	Handshake[10] = 0x00;
	Handshake[11] = 0x00;

	Handshake[12] = 0x00;       //错误代码，固定为0
	Handshake[13] = 0x00;
	Handshake[14] = 0x00;
	Handshake[15] = 0x00;

	Handshake[16] = 0x00;       //
	Handshake[17] = 0x00;       //
	Handshake[18] = 0x00;       //
	Handshake[19] = 0x64;       //m_client端（PC）的IP最后位地址，PC的IP 192.168.250.100
	//首先客户端发送握手信号
	if (m_tcpClient.Send(Handshake, 20) != SE_OK)
	{
		return SE_SEND_FAIL;
	}
	

	const int recvlen = 24;
	char buffer[recvlen] = { 0 };
	int recvLenCnt = 0;
	//接受服务发返回的握手信号响应
	if (m_tcpClient.Recieve(buffer, 24, recvLenCnt) != SE_OK)
	{

		return SE_SEND_FAIL;
	}

	//储存返回的客户端的节点地址
	m_client[0] = buffer[16];
	m_client[1] = buffer[17];
	m_client[2] = buffer[18];
	m_client[3] = buffer[19];
	//储存返回的服务端的节点地址
	m_server[0] = buffer[20];
	m_server[1] = buffer[21];
	m_server[2] = buffer[22];
	m_server[3] = buffer[23];
	finsConnected = true;

	return SE_OK;
}

void LensAA::CloseFins()
{
	m_tcpClient.Close();
}

void LensAA::WriteData(int startAddr, int number, int type, unsigned char* m_data)
{
	int len = 34 + number * 2;
	unsigned char* FullCmd = new unsigned char[len];

	FullCmd[0] = 0x46;      //Main.header
	FullCmd[1] = 0x49;
	FullCmd[2] = 0x4E;
	FullCmd[3] = 0x53;

	unsigned char _3CH[4] = { 0 };
	GetBytes(26 + number * 2, _3CH);
	
	FullCmd[4] = 0x00;      //写入BYTE长度，要从byte[8] 开始算起  0x1A +   
	FullCmd[5] = 0x00;
	FullCmd[6] = _3CH[1];
	FullCmd[7] = _3CH[0];

	FullCmd[8] = 0x00;      //command固定为00 00 00 02     
	FullCmd[9] = 0x00;
	FullCmd[10] = 0x00;
	FullCmd[11] = 0x02;

	FullCmd[12] = 0x00;     //ErrorCode 错误代码固定为00 00 00 00
	FullCmd[13] = 0x00;
	FullCmd[14] = 0x00;
	FullCmd[15] = 0x00;
	//command frame header
	FullCmd[16] = 0x80;         //ICF，发送数据的时候固定是0x80
	FullCmd[17] = 0x00;         //RSV，固定是0
	FullCmd[18] = 0x02;         //GCT，固定是2
	FullCmd[19] = 0x00;         //DNA, 目标网络号，固定是0
	FullCmd[20] = m_server[3];    //DA1，目标节点号，服务端的节点地址
	FullCmd[21] = 0x00;         //DA2, 目标单元号
	FullCmd[22] = 0x00;         //SNA, 源网络号
	FullCmd[23] = m_client[3];    //SA1，源节点号，客户端的IP地址末两位
	FullCmd[24] = 0x00;         //SA2, 源单元号
	FullCmd[25] = 0xFF;         //SID,

	FullCmd[26] = 0x01;         //MR,写入的代码是01 02
	FullCmd[27] = 0x02;         //SR

	//寄存器代码
	unsigned char _CH[4] = { 0 };
	GetBytes(type, _CH);
	FullCmd[28] = _CH[0];

	unsigned char _1CH[4] = { 0 };
	GetBytes(startAddr, _1CH);
	FullCmd[29] = _1CH[1];      //写入的首地址  %D**
	FullCmd[30] = _1CH[0];
	FullCmd[31] = 0x00;         //固定为00

	unsigned char _2CH[4] = { 0 };
	GetBytes(number, _2CH);//写入数量
	FullCmd[32] = _2CH[1];
	FullCmd[33] = _2CH[0];

	for (int i = 0; i < number * 2; i++)
	{
		FullCmd[34 + i] = m_data[i];
	}
	//发送数据
	m_tcpClient.Send((char*)FullCmd, len);
	SAFE_DELETE_ARRAY(FullCmd);
	char buffer[256] = { 0 };
	int recvLenCnt = 0;
	m_tcpClient.Recieve(buffer, 256, recvLenCnt);
}

unsigned char* LensAA::ReadData(int startAddr, int number, int type)
{
	unsigned char* RecData = new unsigned char[256];

	int len = 34;
	unsigned char* FullCmd = new unsigned char[len];
	//TCP Main.header
	FullCmd[0] = 0x46;//F
	FullCmd[1] = 0x49;//I
	FullCmd[2] = 0x4e;//N
	FullCmd[3] = 0x53;//S

	FullCmd[4] = 0x00;         //读取的字节长度，从[8]开始算
	FullCmd[5] = 0x00;
	FullCmd[6] = 0x00;
	FullCmd[7] = 0x1A;
	FullCmd[8] = 0x00;         //COMMAND固定为0x00000002
	FullCmd[9] = 0x00;
	FullCmd[10] = 0x00;
	FullCmd[11] = 0x02;

	FullCmd[12] = 0x00;        //错误代码为0
	FullCmd[13] = 0x00;
	FullCmd[14] = 0x00;
	FullCmd[15] = 0x00;
	//command frame header
	FullCmd[16] = 0x80;         //ICF，发送数据的时候固定是0x80
	FullCmd[17] = 0x00;         //RSV，固定是0
	FullCmd[18] = 0x02;         //GCT，固定是2
	FullCmd[19] = 0x00;         //DNA, 目标网络号，固定是0
	FullCmd[20] = m_server[3];    //DA1，目标节点号，服务端的节点地址
	FullCmd[21] = 0x00;         //DA2, 目标单元号
	FullCmd[22] = 0x00;         //SNA, 源网络号
	FullCmd[23] = m_client[3];    //SA1，源节点号，客户端的IP地址末两位
	FullCmd[24] = 0x00;         //SA2, 源单元号
	FullCmd[25] = 0x00;         //SID,
	FullCmd[26] = 0x01;         //MR
	FullCmd[27] = 0x01;         //SR

	unsigned char _1CH[4] = { 0 };
	GetBytes(type, _1CH);
	FullCmd[28] = _1CH[0];

	GetBytes(startAddr, _1CH);//读取首地址
	FullCmd[29] = _1CH[1];
	FullCmd[30] = _1CH[0];

	FullCmd[31] = 0x00;         //固定为0x00

	GetBytes(number, _1CH);//读取数量
	FullCmd[32] = _1CH[1];
	FullCmd[33] = _1CH[0];

	m_tcpClient.Send((char*)FullCmd, len);      //发送读取数据的请求
	SAFE_DELETE_ARRAY(FullCmd);
	unsigned char buffer[256] = { 0 };              //接收数据
	int recvLenCnt = 0;
	len = 256;
	m_tcpClient.Recieve((char*)buffer, len, recvLenCnt);

	for (short i = 0; i < 56; i++)
	{
		RecData[i * 4] = buffer[31 + i * 4];
		RecData[1 + i * 4] = buffer[30 + i * 4];
		RecData[2 + i * 4] = buffer[33 + i * 4];
		RecData[3 + i * 4] = buffer[32 + i * 4];
	}

	return RecData;
}

void LensAA::GetBytes(int m_data, unsigned char* dst)
{
//	memset(dst, 0, sizeof(unsigned char)*4);
	dst[0] = (0x000000ff&m_data);
	dst[1] = (0x0000ff00&m_data)>>8;
	dst[2] = (0x00ff0000&m_data)>>16;
	dst[3] = (0xff000000&m_data)>>24;
}


int LensAA::ToInt32(unsigned char* src, int index)
{
	unsigned char temp[4] = {0};
	temp[0] = src[index+3];
	temp[1] = src[index+2];
	temp[2] = src[index+1];
	temp[3] = src[index+0];
	int dst = (temp[0]<<24)|(temp[1]<<16)|(temp[2]<<8)|(temp[3]<<0);
	return dst;
}

bool LensAA::SendDword(int startAddr, int contWord)
{
	unsigned char _CH[4] = { 0 };
	unsigned char result[4] = { 0 };
	GetBytes(contWord, _CH);

	result[0] = _CH[1];
	result[1] = _CH[0];
	result[2] = _CH[3];
	result[3] = _CH[2];
	WriteData(startAddr, 2, 0xB1, result);
	return true;
}
bool LensAA::DintSend(int startAddr, int dIntData)
{
	char _CH[4] = { 0 };
	unsigned char result[4] = { 0 };
	sprintf_s(_CH, 4, "%d", dIntData);
	result[0] = _CH[1];
	result[1] = _CH[0];
	result[2] = _CH[3];
	result[3] = _CH[2];
	WriteData(startAddr, 2, 0x82, result);
	return true;
}
bool LensAA::DoubleSend(int startAddr, int axisID, int pos, int len)
{
	int scale = 10000;
	unsigned char* resData = new unsigned char[len * 4];
	unsigned char _1CH[4] = { 0 };
	int n = pos * scale;
	GetBytes(n, _1CH);
	resData[0 + axisID * 4] = _1CH[1];
	resData[1 + axisID * 4] = _1CH[0];
	resData[2 + axisID * 4] = _1CH[3];
	resData[3 + axisID * 4] = _1CH[2];
	WriteData(startAddr, 64, 0x82, resData);
	return true;
}

int LensAA::RecvDword(int startAddr)
{
	unsigned char* RecData = ReadData(startAddr, 2, 0xB1);
	int recvData = ToInt32(RecData, 0);

	return recvData;
}

int LensAA::EnableBit(int src, int index)
{
	return src | (1 << index);
}

int LensAA::DisableBit(int src, int index)
{
	return src & (~(1 << index));
}

int LensAA::ReverseBit(int src, int index)
{
	return src ^ (1 << index);
}

int LensAA::GetBit(int src, int index)
{
	return (src >> index & 1);
}

void LensAA::LoadOption()
{

}

void LensAA::SaveOption()
{
	/*
	//加速度
	WriteConfigString("加速度设置", "前龙门X轴加速度", m_data.AccTime[0].ToString(), filePath);
	WriteConfigString("加速度设置", "前龙门Y轴加速度", m_data.AccTime[1].ToString(), filePath);
	WriteConfigString("加速度设置", "前龙门Z1轴加速度", m_data.AccTime[2].ToString(), filePath);
	WriteConfigString("加速度设置", "前龙门Z2轴加速度", m_data.AccTime[3].ToString(), filePath);
	WriteConfigString("加速度设置", "前龙门Z3轴加速度", m_data.AccTime[4].ToString(), filePath);
	WriteConfigString("加速度设置", "前龙门推杆X轴加速度", m_data.AccTime[5].ToString(), filePath);
	WriteConfigString("加速度设置", "前龙门料盒Y轴加速度", m_data.AccTime[6].ToString(), filePath);
	WriteConfigString("加速度设置", "前龙门料盒Z轴加速度", m_data.AccTime[7].ToString(), filePath);
	WriteConfigString("加速度设置", "工位一X轴加速度", m_data.AccTime[8].ToString(), filePath);
	WriteConfigString("加速度设置", "工位一Y轴加速度", m_data.AccTime[9].ToString(), filePath);
	WriteConfigString("加速度设置", "工位一大负载Z1轴加速度", m_data.AccTime[10].ToString(), filePath);
	WriteConfigString("加速度设置", "工位一小负载Z2轴加速度", m_data.AccTime[11].ToString(), filePath);
	WriteConfigString("加速度设置", "工位一治具Z3轴加速度", m_data.AccTime[12].ToString(), filePath);
	WriteConfigString("加速度设置", "工位一R轴加速度", m_data.AccTime[13].ToString(), filePath);
	WriteConfigString("加速度设置", "工位二X轴加速度", m_data.AccTime[14].ToString(), filePath);
	WriteConfigString("加速度设置", "工位二Y轴加速度", m_data.AccTime[15].ToString(), filePath);
	WriteConfigString("加速度设置", "工位二大负载Z1轴加速度", m_data.AccTime[16].ToString(), filePath);
	WriteConfigString("加速度设置", "工位二小负载Z2轴加速度", m_data.AccTime[17].ToString(), filePath);
	WriteConfigString("加速度设置", "工位二治具Z3轴加速度", m_data.AccTime[18].ToString(), filePath);
	WriteConfigString("加速度设置", "工位二R轴加速度", m_data.AccTime[19].ToString(), filePath);
	WriteConfigString("加速度设置", "后龙门X轴加速度", m_data.AccTime[20].ToString(), filePath);
	WriteConfigString("加速度设置", "后龙门Y轴加速度", m_data.AccTime[21].ToString(), filePath);
	WriteConfigString("加速度设置", "后龙门Z轴加速度", m_data.AccTime[22].ToString(), filePath);
	WriteConfigString("加速度设置", "后龙门料盒Y轴加速度", m_data.AccTime[23].ToString(), filePath);


	//减速度
	WriteConfigString("减速度设置", "前龙门X轴减速度", m_data.DecTime[0].ToString(), filePath);
	WriteConfigString("减速度设置", "前龙门Y轴减速度", m_data.DecTime[1].ToString(), filePath);
	WriteConfigString("减速度设置", "前龙门Z1轴减速度", m_data.DecTime[2].ToString(), filePath);
	WriteConfigString("减速度设置", "前龙门Z2轴减速度", m_data.DecTime[3].ToString(), filePath);
	WriteConfigString("减速度设置", "前龙门Z3轴减速度", m_data.DecTime[4].ToString(), filePath);
	WriteConfigString("减速度设置", "前龙门推杆X轴减速度", m_data.DecTime[5].ToString(), filePath);
	WriteConfigString("减速度设置", "前龙门料盒Y轴减速度", m_data.DecTime[6].ToString(), filePath);
	WriteConfigString("减速度设置", "前龙门料盒Z轴减速度", m_data.DecTime[7].ToString(), filePath);
	WriteConfigString("减速度设置", "工位一X轴减速度", m_data.DecTime[8].ToString(), filePath);
	WriteConfigString("减速度设置", "工位一Y轴减速度", m_data.DecTime[9].ToString(), filePath);
	WriteConfigString("减速度设置", "工位一大负载Z1轴减速度", m_data.DecTime[10].ToString(), filePath);
	WriteConfigString("减速度设置", "工位一小负载Z2轴减速度", m_data.DecTime[11].ToString(), filePath);
	WriteConfigString("减速度设置", "工位一治具Z3轴减速度", m_data.DecTime[12].ToString(), filePath);
	WriteConfigString("减速度设置", "工位一R轴减速度", m_data.DecTime[13].ToString(), filePath);
	WriteConfigString("减速度设置", "工位二X轴减速度", m_data.DecTime[14].ToString(), filePath);
	WriteConfigString("减速度设置", "工位二Y轴减速度", m_data.DecTime[15].ToString(), filePath);
	WriteConfigString("减速度设置", "工位二大负载Z1轴减速度", m_data.DecTime[16].ToString(), filePath);
	WriteConfigString("减速度设置", "工位二小负载Z2轴减速度", m_data.DecTime[17].ToString(), filePath);
	WriteConfigString("减速度设置", "工位二治具Z3轴减速度", m_data.DecTime[18].ToString(), filePath);
	WriteConfigString("减速度设置", "工位二R轴减速度", m_data.DecTime[19].ToString(), filePath);
	WriteConfigString("减速度设置", "后龙门X轴减速度", m_data.DecTime[20].ToString(), filePath);
	WriteConfigString("减速度设置", "后龙门Y轴减速度", m_data.DecTime[21].ToString(), filePath);
	WriteConfigString("减速度设置", "后龙门Z轴减速度", m_data.DecTime[22].ToString(), filePath);
	WriteConfigString("减速度设置", "后龙门料盒Y轴减速度", m_data.DecTime[23].ToString(), filePath);

	// JOG运行速度
	WriteConfigString("JOG速度设置", "前龙门X轴JOG速度", m_data.JogVel[0].ToString(), filePath);
	WriteConfigString("JOG速度设置", "前龙门Y轴JOG速度", m_data.JogVel[1].ToString(), filePath);
	WriteConfigString("JOG速度设置", "前龙门Z1轴JOG速度", m_data.JogVel[2].ToString(), filePath);
	WriteConfigString("JOG速度设置", "前龙门Z2轴JOG速度", m_data.JogVel[3].ToString(), filePath);
	WriteConfigString("JOG速度设置", "前龙门Z3轴JOG速度", m_data.JogVel[4].ToString(), filePath);
	WriteConfigString("JOG速度设置", "前龙门推杆X轴JOG速度", m_data.JogVel[5].ToString(), filePath);
	WriteConfigString("JOG速度设置", "前龙门料盒Y轴JOG速度", m_data.JogVel[6].ToString(), filePath);
	WriteConfigString("JOG速度设置", "前龙门料盒Z轴JOG速度", m_data.JogVel[7].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位一X轴JOG速度", m_data.JogVel[8].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位一Y轴JOG速度", m_data.JogVel[9].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位一大负载Z1轴JOG速度", m_data.JogVel[10].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位一小负载Z2轴JOG速度", m_data.JogVel[11].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位一治具Z3轴JOG速度", m_data.JogVel[12].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位一R轴JOG速度", m_data.JogVel[13].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位二X轴JOG速度", m_data.JogVel[14].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位二Y轴JOG速度", m_data.JogVel[15].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位二大负载Z1轴JOG速度", m_data.JogVel[16].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位二小负载Z2轴JOG速度", m_data.JogVel[17].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位二治具Z3轴JOG速度", m_data.JogVel[18].ToString(), filePath);
	WriteConfigString("JOG速度设置", "工位二R轴JOG速度", m_data.JogVel[19].ToString(), filePath);
	WriteConfigString("JOG速度设置", "后龙门X轴JOG速度", m_data.JogVel[20].ToString(), filePath);
	WriteConfigString("JOG速度设置", "后龙门Y轴JOG速度", m_data.JogVel[21].ToString(), filePath);
	WriteConfigString("JOG速度设置", "后龙门Z轴JOG速度", m_data.JogVel[22].ToString(), filePath);
	WriteConfigString("JOG速度设置", "后龙门料盒Y轴JOG速度", m_data.JogVel[23].ToString(), filePath);


	//运行速度
	WriteConfigString("运行速度设置", "前龙门X轴运行速度", m_data.Vel[0].ToString(), filePath);
	WriteConfigString("运行速度设置", "前龙门Y轴运行速度", m_data.Vel[1].ToString(), filePath);
	WriteConfigString("运行速度设置", "前龙门Z1轴运行速度", m_data.Vel[2].ToString(), filePath);
	WriteConfigString("运行速度设置", "前龙门Z2轴运行速度", m_data.Vel[3].ToString(), filePath);
	WriteConfigString("运行速度设置", "前龙门Z3轴运行速度", m_data.Vel[4].ToString(), filePath);
	WriteConfigString("运行速度设置", "前龙门推杆X轴运行速度", m_data.Vel[5].ToString(), filePath);
	WriteConfigString("运行速度设置", "前龙门料盒Y轴运行速度", m_data.Vel[6].ToString(), filePath);
	WriteConfigString("运行速度设置", "前龙门料盒Z轴运行速度", m_data.Vel[7].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位一X轴运行速度", m_data.Vel[8].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位一Y轴运行速度", m_data.Vel[9].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位一大负载Z1轴运行速度", m_data.Vel[10].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位一小负载Z2轴运行速度", m_data.Vel[11].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位一治具Z3轴运行速度", m_data.Vel[12].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位一R轴运行速度", m_data.Vel[13].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位二X轴运行速度", m_data.Vel[14].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位二Y轴运行速度", m_data.Vel[15].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位二大负载Z1轴运行速度", m_data.Vel[16].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位二小负载Z2轴运行速度", m_data.Vel[17].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位二治具Z3轴运行速度", m_data.Vel[18].ToString(), filePath);
	WriteConfigString("运行速度设置", "工位二R轴运行速度", m_data.Vel[19].ToString(), filePath);
	WriteConfigString("运行速度设置", "后龙门X轴运行速度", m_data.Vel[20].ToString(), filePath);
	WriteConfigString("运行速度设置", "后龙门Y轴运行速度", m_data.Vel[21].ToString(), filePath);
	WriteConfigString("运行速度设置", "后龙门Z轴运行速度", m_data.Vel[22].ToString(), filePath);
	WriteConfigString("运行速度设置", "后龙门料盒Y轴运行速度", m_data.Vel[23].ToString(), filePath);




	//前龙门点位保存
	WriteConfigString("前龙门料盘拍照示教位1", "X轴", m_data.PointQ[0].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位1", "Y轴", m_data.PointQ[1].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位1", "Z轴", m_data.PointQ[2].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位2", "X轴", m_data.PointQ[3].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位2", "Y轴", m_data.PointQ[4].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位2", "Z轴", m_data.PointQ[5].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位3", "X轴", m_data.PointQ[6].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位3", "Y轴", m_data.PointQ[7].ToString(), filePath);
	WriteConfigString("前龙门料盘拍照示教位3", "Z轴", m_data.PointQ[8].ToString(), filePath);

	WriteConfigString("前龙门OK放料示教位1", "X轴", m_data.PointQ[9].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位1", "Y轴", m_data.PointQ[10].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位1", "Z轴", m_data.PointQ[11].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位2", "X轴", m_data.PointQ[12].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位2", "Y轴", m_data.PointQ[13].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位2", "Z轴", m_data.PointQ[14].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位3", "X轴", m_data.PointQ[15].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位3", "Y轴", m_data.PointQ[16].ToString(), filePath);
	WriteConfigString("前龙门OK放料示教位3", "Z轴", m_data.PointQ[17].ToString(), filePath);

	WriteConfigString("前龙门NG放料示教位1", "X轴", m_data.PointQ[18].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位1", "Y轴", m_data.PointQ[19].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位1", "Z轴", m_data.PointQ[20].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位2", "X轴", m_data.PointQ[21].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位2", "Y轴", m_data.PointQ[22].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位2", "Z轴", m_data.PointQ[23].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位3", "X轴", m_data.PointQ[24].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位3", "Y轴", m_data.PointQ[25].ToString(), filePath);
	WriteConfigString("前龙门NG放料示教位3", "Z轴", m_data.PointQ[26].ToString(), filePath);

	WriteConfigString("前龙门下相机拍照位", "X轴", m_data.PointQ[27].ToString(), filePath);
	WriteConfigString("前龙门下相机拍照位", "Y轴", m_data.PointQ[28].ToString(), filePath);
	WriteConfigString("前龙门下相机拍照位", "Z轴", m_data.PointQ[29].ToString(), filePath);

	WriteConfigString("前龙门工位一下料位", "X轴", m_data.PointQ[30].ToString(), filePath);
	WriteConfigString("前龙门工位一下料位", "Y轴", m_data.PointQ[31].ToString(), filePath);
	WriteConfigString("前龙门工位一下料位", "Z轴", m_data.PointQ[32].ToString(), filePath);

	WriteConfigString("前龙门工位一上料位", "X轴", m_data.PointQ[33].ToString(), filePath);
	WriteConfigString("前龙门工位一上料位", "Y轴", m_data.PointQ[34].ToString(), filePath);
	WriteConfigString("前龙门工位一上料位", "Z轴", m_data.PointQ[35].ToString(), filePath);

	WriteConfigString("前龙门工位二下料位", "X轴", m_data.PointQ[36].ToString(), filePath);
	WriteConfigString("前龙门工位二下料位", "Y轴", m_data.PointQ[37].ToString(), filePath);
	WriteConfigString("前龙门工位二下料位", "Z轴", m_data.PointQ[38].ToString(), filePath);

	WriteConfigString("前龙门工位二上料位", "X轴", m_data.PointQ[39].ToString(), filePath);
	WriteConfigString("前龙门工位二上料位", "Y轴", m_data.PointQ[40].ToString(), filePath);
	WriteConfigString("前龙门工位二上料位", "Z轴", m_data.PointQ[41].ToString(), filePath);

	WriteConfigString("前龙门推杆夹料盘位", "X轴", m_data.PointQ[45].ToString(), filePath);
	WriteConfigString("前龙门推杆料盘到位", "X轴", m_data.PointQ[48].ToString(), filePath);

	WriteConfigString("前龙门料盘上下料位", "Y轴", m_data.PointQ[58].ToString(), filePath);

	WriteConfigString("前龙门弹夹上下料位", "Z轴", m_data.PointQ[65].ToString(), filePath);
	WriteConfigString("前龙门弹夹首层位置", "Z轴", m_data.PointQ[68].ToString(), filePath);
	WriteConfigString("前龙门弹夹末层位置", "Z轴", m_data.PointQ[71].ToString(), filePath);


	//工位一点位保存
	WriteConfigString("工位一Lens位置", "X轴", m_data.PointG1[0].ToString(), filePath);
	WriteConfigString("工位一Lens位置", "Y轴", m_data.PointG1[1].ToString(), filePath);
	WriteConfigString("工位一Lens位置", "Z1轴", m_data.PointG1[2].ToString(), filePath);
	WriteConfigString("工位一Lens位置", "Z2轴", m_data.PointG1[3].ToString(), filePath);

	WriteConfigString("工位一Cone位置", "X轴", m_data.PointG1[4].ToString(), filePath);
	WriteConfigString("工位一Cone位置", "Y轴", m_data.PointG1[5].ToString(), filePath);
	WriteConfigString("工位一Cone位置", "Z1轴", m_data.PointG1[6].ToString(), filePath);
	WriteConfigString("工位一Cone位置", "Z2轴", m_data.PointG1[7].ToString(), filePath);

	WriteConfigString("工位一拍照位置", "X轴", m_data.PointG1[8].ToString(), filePath);
	WriteConfigString("工位一拍照位置", "Y轴", m_data.PointG1[9].ToString(), filePath);
	WriteConfigString("工位一拍照位置", "Z1轴", m_data.PointG1[10].ToString(), filePath);
	WriteConfigString("工位一拍照位置", "Z2轴", m_data.PointG1[11].ToString(), filePath);

	WriteConfigString("工位一点胶拍照位置", "X轴", m_data.PointG1[12].ToString(), filePath);
	WriteConfigString("工位一点胶拍照位置", "Y轴", m_data.PointG1[13].ToString(), filePath);
	WriteConfigString("工位一点胶拍照位置", "Z1轴", m_data.PointG1[14].ToString(), filePath);
	WriteConfigString("工位一点胶拍照位置", "Z2轴", m_data.PointG1[15].ToString(), filePath);

	WriteConfigString("工位一对位视觉补偿位", "X轴", m_data.PointG1[16].ToString(), filePath);
	WriteConfigString("工位一对位视觉补偿位", "Y轴", m_data.PointG1[17].ToString(), filePath);
	WriteConfigString("工位一对位视觉补偿位", "Z1轴", m_data.PointG1[18].ToString(), filePath);
	WriteConfigString("工位一对位视觉补偿位", "Z2轴", m_data.PointG1[19].ToString(), filePath);

	WriteConfigString("工位一NG下料位", "X轴", m_data.PointG1[20].ToString(), filePath);
	WriteConfigString("工位一NG下料位", "Y轴", m_data.PointG1[21].ToString(), filePath);
	WriteConfigString("工位一NG下料位", "Z1轴", m_data.PointG1[22].ToString(), filePath);
	WriteConfigString("工位一NG下料位", "Z2轴", m_data.PointG1[23].ToString(), filePath);

	WriteConfigString("工位一手动直线插补", "X轴", m_data.PointG1[24].ToString(), filePath);
	WriteConfigString("工位一手动直线插补", "Y轴", m_data.PointG1[25].ToString(), filePath);
	WriteConfigString("工位一手动直线插补", "Z1轴", m_data.PointG1[26].ToString(), filePath);
	WriteConfigString("工位一手动直线插补", "Z2轴", m_data.PointG1[27].ToString(), filePath);

	WriteConfigString("工位一AA参数", "旋转R轴初始位", m_data.PointG1[40].ToString(), filePath);     //工位一AA参数
	WriteConfigString("工位一AA参数", "旋转R轴步长值", m_data.PointG1[41].ToString(), filePath);
	WriteConfigString("工位一AA参数", "治具Z3轴初始位", m_data.PointG1[42].ToString(), filePath);
	WriteConfigString("工位一AA参数", "治具Z3轴步长值", m_data.PointG1[43].ToString(), filePath);
	WriteConfigString("工位一AA参数", "X轴步长值", m_data.PointG1[44].ToString(), filePath);
	WriteConfigString("工位一AA参数", "Y轴步长值", m_data.PointG1[45].ToString(), filePath);


	//工位二点位保存
	WriteConfigString("工位二Lens位置", "X轴", m_data.PointG2[0].ToString(), filePath);
	WriteConfigString("工位二Lens位置", "Y轴", m_data.PointG2[1].ToString(), filePath);
	WriteConfigString("工位二Lens位置", "Z1轴", m_data.PointG2[2].ToString(), filePath);
	WriteConfigString("工位二Lens位置", "Z2轴", m_data.PointG2[3].ToString(), filePath);

	WriteConfigString("工位二Cone位置", "X轴", m_data.PointG2[4].ToString(), filePath);
	WriteConfigString("工位二Cone位置", "Y轴", m_data.PointG2[5].ToString(), filePath);
	WriteConfigString("工位二Cone位置", "Z1轴", m_data.PointG2[6].ToString(), filePath);
	WriteConfigString("工位二Cone位置", "Z2轴", m_data.PointG2[7].ToString(), filePath);

	WriteConfigString("工位二拍照位置", "X轴", m_data.PointG2[8].ToString(), filePath);
	WriteConfigString("工位二拍照位置", "Y轴", m_data.PointG2[9].ToString(), filePath);
	WriteConfigString("工位二拍照位置", "Z1轴", m_data.PointG2[10].ToString(), filePath);
	WriteConfigString("工位二拍照位置", "Z2轴", m_data.PointG2[11].ToString(), filePath);

	WriteConfigString("工位二点胶拍照位置", "X轴", m_data.PointG2[12].ToString(), filePath);
	WriteConfigString("工位二点胶拍照位置", "Y轴", m_data.PointG2[13].ToString(), filePath);
	WriteConfigString("工位二点胶拍照位置", "Z1轴", m_data.PointG2[14].ToString(), filePath);
	WriteConfigString("工位二点胶拍照位置", "Z2轴", m_data.PointG2[15].ToString(), filePath);

	WriteConfigString("工位二对位视觉补偿位", "X轴", m_data.PointG2[16].ToString(), filePath);
	WriteConfigString("工位二对位视觉补偿位", "Y轴", m_data.PointG2[17].ToString(), filePath);
	WriteConfigString("工位二对位视觉补偿位", "Z1轴", m_data.PointG2[18].ToString(), filePath);
	WriteConfigString("工位二对位视觉补偿位", "Z2轴", m_data.PointG2[19].ToString(), filePath);

	WriteConfigString("工位二NG下料位", "X轴", m_data.PointG2[20].ToString(), filePath);
	WriteConfigString("工位二NG下料位", "Y轴", m_data.PointG2[21].ToString(), filePath);
	WriteConfigString("工位二NG下料位", "Z1轴", m_data.PointG2[22].ToString(), filePath);
	WriteConfigString("工位二NG下料位", "Z2轴", m_data.PointG2[23].ToString(), filePath);

	WriteConfigString("工位二手动直线插补", "X轴", m_data.PointG2[24].ToString(), filePath);
	WriteConfigString("工位二手动直线插补", "Y轴", m_data.PointG2[25].ToString(), filePath);
	WriteConfigString("工位二手动直线插补", "Z1轴", m_data.PointG2[26].ToString(), filePath);
	WriteConfigString("工位二手动直线插补", "Z2轴", m_data.PointG2[27].ToString(), filePath);

	WriteConfigString("工位二AA参数", "旋转R轴初始位", m_data.PointG2[40].ToString(), filePath); //工位二AA参数
	WriteConfigString("工位二AA参数", "旋转R轴步长值", m_data.PointG2[41].ToString(), filePath);
	WriteConfigString("工位二AA参数", "治具Z3轴初始位", m_data.PointG2[42].ToString(), filePath);
	WriteConfigString("工位二AA参数", "治具Z3轴步长值", m_data.PointG2[43].ToString(), filePath);
	WriteConfigString("工位二AA参数", "X轴步长值", m_data.PointG2[44].ToString(), filePath);
	WriteConfigString("工位二AA参数", "Y轴步长值", m_data.PointG2[45].ToString(), filePath);


	//后龙门点位保存
	WriteConfigString("后龙门料盘拍照示教位1", "X轴", m_data.PointH[0].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位1", "Y轴", m_data.PointH[1].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位1", "Z轴", m_data.PointH[2].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位2", "X轴", m_data.PointH[3].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位2", "Y轴", m_data.PointH[4].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位2", "Z轴", m_data.PointH[5].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位3", "X轴", m_data.PointH[6].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位3", "Y轴", m_data.PointH[7].ToString(), filePath);
	WriteConfigString("后龙门料盘拍照示教位3", "Z轴", m_data.PointH[8].ToString(), filePath);

	WriteConfigString("后龙门下相机拍照位", "X轴", m_data.PointH[27].ToString(), filePath);
	WriteConfigString("后龙门下相机拍照位", "Y轴", m_data.PointH[28].ToString(), filePath);
	WriteConfigString("后龙门下相机拍照位", "Z轴", m_data.PointH[29].ToString(), filePath);

	WriteConfigString("后龙门工位一下料位", "X轴", m_data.PointH[30].ToString(), filePath);
	WriteConfigString("后龙门工位一下料位", "Y轴", m_data.PointH[31].ToString(), filePath);
	WriteConfigString("后龙门工位一下料位", "Z轴", m_data.PointH[32].ToString(), filePath);

	WriteConfigString("后龙门工位一上料位", "X轴", m_data.PointH[33].ToString(), filePath);
	WriteConfigString("后龙门工位一上料位", "Y轴", m_data.PointH[34].ToString(), filePath);
	WriteConfigString("后龙门工位一上料位", "Z轴", m_data.PointH[35].ToString(), filePath);

	WriteConfigString("后龙门工位二下料位", "X轴", m_data.PointH[36].ToString(), filePath);
	WriteConfigString("后龙门工位二下料位", "Y轴", m_data.PointH[37].ToString(), filePath);
	WriteConfigString("后龙门工位二下料位", "Z轴", m_data.PointH[38].ToString(), filePath);

	WriteConfigString("后龙门工位二上料位", "X轴", m_data.PointH[39].ToString(), filePath);
	WriteConfigString("后龙门工位二上料位", "Y轴", m_data.PointH[40].ToString(), filePath);
	WriteConfigString("后龙门工位二上料位", "Z轴", m_data.PointH[41].ToString(), filePath);

	WriteConfigString("后龙门料盘上下料位", "Y轴", m_data.PointH[58].ToString(), filePath);


	//电气比例阀参数
	WriteConfigString("前龙门电气比例阀参数", "Z1吸嘴吸料盘", m_data.AnalogPara[0].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z1工位一上料", m_data.AnalogPara[1].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z1工位二上料", m_data.AnalogPara[2].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z1手动设置模拟值", m_data.AnalogPara[3].ToString(), filePath);

	WriteConfigString("前龙门电气比例阀参数", "Z2吸嘴吸料盘", m_data.AnalogPara[5].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z2工位一上料", m_data.AnalogPara[6].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z2工位二上料", m_data.AnalogPara[7].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z2手动设置模拟值", m_data.AnalogPara[8].ToString(), filePath);

	WriteConfigString("前龙门电气比例阀参数", "Z3吸嘴吸料盘", m_data.AnalogPara[10].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z3工位一上料", m_data.AnalogPara[11].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z3工位二上料", m_data.AnalogPara[12].ToString(), filePath);
	WriteConfigString("前龙门电气比例阀参数", "Z3手动设置模拟值", m_data.AnalogPara[13].ToString(), filePath);

	WriteConfigString("工位一电气比例阀参数", "小负载吸嘴", m_data.AnalogPara[15].ToString(), filePath);
	WriteConfigString("工位一电气比例阀参数", "大负载AA前", m_data.AnalogPara[16].ToString(), filePath);
	WriteConfigString("工位一电气比例阀参数", "大负载AA后", m_data.AnalogPara[17].ToString(), filePath);
	WriteConfigString("工位一电气比例阀参数", "手动设置模拟值", m_data.AnalogPara[18].ToString(), filePath);

	WriteConfigString("工位二电气比例阀参数", "小负载吸嘴", m_data.AnalogPara[20].ToString(), filePath);
	WriteConfigString("工位二电气比例阀参数", "大负载AA前", m_data.AnalogPara[21].ToString(), filePath);
	WriteConfigString("工位二电气比例阀参数", "大负载AA后", m_data.AnalogPara[22].ToString(), filePath);
	WriteConfigString("工位二电气比例阀参数", "手动设置模拟值", m_data.AnalogPara[23].ToString(), filePath);

	WriteConfigString("后龙门电气比例阀参数", "吸嘴吸料盘", m_data.AnalogPara[25].ToString(), filePath);
	WriteConfigString("后龙门电气比例阀参数", "工位一上料", m_data.AnalogPara[26].ToString(), filePath);
	WriteConfigString("后龙门电气比例阀参数", "工位二上料", m_data.AnalogPara[27].ToString(), filePath);
	WriteConfigString("后龙门电气比例阀参数", "手动设置模拟值", m_data.AnalogPara[28].ToString(), filePath);
	*/
}

void LensAA::WriteOption()
{
	WriteAccTime();     //刷新写入轴加速度
	WriteDecTime();     //刷新写入轴减速度
	WriteJogVel();      //刷新写入JOG速度
	WriteVel();         //刷新写入定位速度
	WritePos();         //刷新写入定位地址

						//标志位Symbol =1 ；PLC根据MSymbol的值进行数据的最终传送
	DintSend(0, 1);
}

void LensAA::ReadOption()
{
	/*
	//加速度
	m_data.AccTime[0] = DECIMAL(ReadConfigString("加速度设置", "前龙门X轴加速度", filePath));
	m_data.AccTime[1] = DECIMAL(ReadConfigString("加速度设置", "前龙门Y轴加速度", filePath));
	m_data.AccTime[2] = DECIMAL(ReadConfigString("加速度设置", "前龙门Z1轴加速度", filePath));
	m_data.AccTime[3] = DECIMAL(ReadConfigString("加速度设置", "前龙门Z2轴加速度", filePath));
	m_data.AccTime[4] = DECIMAL(ReadConfigString("加速度设置", "前龙门Z3轴加速度", filePath));
	m_data.AccTime[5] = DECIMAL(ReadConfigString("加速度设置", "前龙门推杆X轴加速度", filePath));
	m_data.AccTime[6] = DECIMAL(ReadConfigString("加速度设置", "前龙门料盒Y轴加速度", filePath));
	m_data.AccTime[7] = DECIMAL(ReadConfigString("加速度设置", "前龙门料盒Z轴加速度", filePath));
	m_data.AccTime[8] = DECIMAL(ReadConfigString("加速度设置", "工位一X轴加速度", filePath));
	m_data.AccTime[9] = DECIMAL(ReadConfigString("加速度设置", "工位一Y轴加速度", filePath));
	m_data.AccTime[10] = DECIMAL(ReadConfigString("加速度设置", "工位一大负载Z1轴加速度", filePath));
	m_data.AccTime[11] = DECIMAL(ReadConfigString("加速度设置", "工位一小负载Z2轴加速度", filePath));
	m_data.AccTime[12] = DECIMAL(ReadConfigString("加速度设置", "工位一治具Z3轴加速度", filePath));
	m_data.AccTime[13] = DECIMAL(ReadConfigString("加速度设置", "工位一R轴加速度", filePath));
	m_data.AccTime[14] = DECIMAL(ReadConfigString("加速度设置", "工位二X轴加速度", filePath));
	m_data.AccTime[15] = DECIMAL(ReadConfigString("加速度设置", "工位二Y轴加速度", filePath));
	m_data.AccTime[16] = DECIMAL(ReadConfigString("加速度设置", "工位二大负载Z1轴加速度", filePath));
	m_data.AccTime[17] = DECIMAL(ReadConfigString("加速度设置", "工位二小负载Z2轴加速度", filePath));
	m_data.AccTime[18] = DECIMAL(ReadConfigString("加速度设置", "工位二治具Z3轴加速度", filePath));
	m_data.AccTime[19] = DECIMAL(ReadConfigString("加速度设置", "工位二R轴加速度", filePath));
	m_data.AccTime[20] = DECIMAL(ReadConfigString("加速度设置", "后龙门X轴加速度", filePath));
	m_data.AccTime[21] = DECIMAL(ReadConfigString("加速度设置", "后龙门Y轴加速度", filePath));
	m_data.AccTime[22] = DECIMAL(ReadConfigString("加速度设置", "后龙门Z轴加速度", filePath));
	m_data.AccTime[23] = DECIMAL(ReadConfigString("加速度设置", "后龙门料盒Y轴加速度", filePath));


	//减速度
	m_data.DecTime[0] = DECIMAL(ReadConfigString("减速度设置", "前龙门X轴减速度", filePath));
	m_data.DecTime[1] = DECIMAL(ReadConfigString("减速度设置", "前龙门Y轴减速度", filePath));
	m_data.DecTime[2] = DECIMAL(ReadConfigString("减速度设置", "前龙门Z1轴减速度", filePath));
	m_data.DecTime[3] = DECIMAL(ReadConfigString("减速度设置", "前龙门Z2轴减速度", filePath));
	m_data.DecTime[4] = DECIMAL(ReadConfigString("减速度设置", "前龙门Z3轴减速度", filePath));
	m_data.DecTime[5] = DECIMAL(ReadConfigString("减速度设置", "前龙门推杆X轴减速度", filePath));
	m_data.DecTime[6] = DECIMAL(ReadConfigString("减速度设置", "前龙门料盒Y轴减速度", filePath));
	m_data.DecTime[7] = DECIMAL(ReadConfigString("减速度设置", "前龙门料盒Z轴减速度", filePath));
	m_data.DecTime[8] = DECIMAL(ReadConfigString("减速度设置", "工位一X轴减速度", filePath));
	m_data.DecTime[9] = DECIMAL(ReadConfigString("减速度设置", "工位一Y轴减速度", filePath));
	m_data.DecTime[10] = DECIMAL(ReadConfigString("减速度设置", "工位一大负载Z1轴减速度", filePath));
	m_data.DecTime[11] = DECIMAL(ReadConfigString("减速度设置", "工位一小负载Z2轴减速度", filePath));
	m_data.DecTime[12] = DECIMAL(ReadConfigString("减速度设置", "工位一治具Z3轴减速度", filePath));
	m_data.DecTime[13] = DECIMAL(ReadConfigString("减速度设置", "工位一R轴减速度", filePath));
	m_data.DecTime[14] = DECIMAL(ReadConfigString("减速度设置", "工位二X轴减速度", filePath));
	m_data.DecTime[15] = DECIMAL(ReadConfigString("减速度设置", "工位二Y轴减速度", filePath));
	m_data.DecTime[16] = DECIMAL(ReadConfigString("减速度设置", "工位二大负载Z1轴减速度", filePath));
	m_data.DecTime[17] = DECIMAL(ReadConfigString("减速度设置", "工位二小负载Z2轴减速度", filePath));
	m_data.DecTime[18] = DECIMAL(ReadConfigString("减速度设置", "工位二治具Z3轴减速度", filePath));
	m_data.DecTime[19] = DECIMAL(ReadConfigString("减速度设置", "工位二R轴减速度", filePath));
	m_data.DecTime[20] = DECIMAL(ReadConfigString("减速度设置", "后龙门X轴减速度", filePath));
	m_data.DecTime[21] = DECIMAL(ReadConfigString("减速度设置", "后龙门Y轴减速度", filePath));
	m_data.DecTime[22] = DECIMAL(ReadConfigString("减速度设置", "后龙门Z轴减速度", filePath));
	m_data.DecTime[23] = DECIMAL(ReadConfigString("减速度设置", "后龙门料盒Y轴减速度", filePath));

	//JOG运动速度
	m_data.JogVel[0] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门X轴JOG速度", filePath));
	m_data.JogVel[1] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门Y轴JOG速度", filePath));
	m_data.JogVel[2] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门Z1轴JOG速度", filePath));
	m_data.JogVel[3] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门Z2轴JOG速度", filePath));
	m_data.JogVel[4] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门Z3轴JOG速度", filePath));
	m_data.JogVel[5] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门推杆X轴JOG速度", filePath));
	m_data.JogVel[6] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门料盒Y轴JOG速度", filePath));
	m_data.JogVel[7] = DECIMAL(ReadConfigString("JOG速度设置", "前龙门料盒Z轴JOG速度", filePath));
	m_data.JogVel[8] = DECIMAL(ReadConfigString("JOG速度设置", "工位一X轴JOG速度", filePath));
	m_data.JogVel[9] = DECIMAL(ReadConfigString("JOG速度设置", "工位一Y轴JOG速度", filePath));
	m_data.JogVel[10] = DECIMAL(ReadConfigString("JOG速度设置", "工位一大负载Z1轴JOG速度", filePath));
	m_data.JogVel[11] = DECIMAL(ReadConfigString("JOG速度设置", "工位一小负载Z2轴JOG速度", filePath));
	m_data.JogVel[12] = DECIMAL(ReadConfigString("JOG速度设置", "工位一治具Z3轴JOG速度", filePath));
	m_data.JogVel[13] = DECIMAL(ReadConfigString("JOG速度设置", "工位一R轴JOG速度", filePath));
	m_data.JogVel[14] = DECIMAL(ReadConfigString("JOG速度设置", "工位二X轴JOG速度", filePath));
	m_data.JogVel[15] = DECIMAL(ReadConfigString("JOG速度设置", "工位二Y轴JOG速度", filePath));
	m_data.JogVel[16] = DECIMAL(ReadConfigString("JOG速度设置", "工位二大负载Z1轴JOG速度", filePath));
	m_data.JogVel[17] = DECIMAL(ReadConfigString("JOG速度设置", "工位二小负载Z2轴JOG速度", filePath));
	m_data.JogVel[18] = DECIMAL(ReadConfigString("JOG速度设置", "工位二治具Z3轴JOG速度", filePath));
	m_data.JogVel[19] = DECIMAL(ReadConfigString("JOG速度设置", "工位二R轴JOG速度", filePath));
	m_data.JogVel[20] = DECIMAL(ReadConfigString("JOG速度设置", "后龙门X轴JOG速度", filePath));
	m_data.JogVel[21] = DECIMAL(ReadConfigString("JOG速度设置", "后龙门Y轴JOG速度", filePath));
	m_data.JogVel[22] = DECIMAL(ReadConfigString("JOG速度设置", "后龙门Z轴JOG速度", filePath));
	m_data.JogVel[23] = DECIMAL(ReadConfigString("JOG速度设置", "后龙门料盒Y轴JOG速度", filePath));


	//运动速度
	m_data.Vel[0] = DECIMAL(ReadConfigString("运行速度设置", "前龙门X轴运行速度", filePath));
	m_data.Vel[1] = DECIMAL(ReadConfigString("运行速度设置", "前龙门Y轴运行速度", filePath));
	m_data.Vel[2] = DECIMAL(ReadConfigString("运行速度设置", "前龙门Z1轴运行速度", filePath));
	m_data.Vel[3] = DECIMAL(ReadConfigString("运行速度设置", "前龙门Z2轴运行速度", filePath));
	m_data.Vel[4] = DECIMAL(ReadConfigString("运行速度设置", "前龙门Z3轴运行速度", filePath));
	m_data.Vel[5] = DECIMAL(ReadConfigString("运行速度设置", "前龙门推杆X轴运行速度", filePath));
	m_data.Vel[6] = DECIMAL(ReadConfigString("运行速度设置", "前龙门料盒Y轴运行速度", filePath));
	m_data.Vel[7] = DECIMAL(ReadConfigString("运行速度设置", "前龙门料盒Z轴运行速度", filePath));
	m_data.Vel[8] = DECIMAL(ReadConfigString("运行速度设置", "工位一X轴运行速度", filePath));
	m_data.Vel[9] = DECIMAL(ReadConfigString("运行速度设置", "工位一Y轴运行速度", filePath));
	m_data.Vel[10] = DECIMAL(ReadConfigString("运行速度设置", "工位一大负载Z1轴运行速度", filePath));
	m_data.Vel[11] = DECIMAL(ReadConfigString("运行速度设置", "工位一小负载Z2轴运行速度", filePath));
	m_data.Vel[12] = DECIMAL(ReadConfigString("运行速度设置", "工位一治具Z3轴运行速度", filePath));
	m_data.Vel[13] = DECIMAL(ReadConfigString("运行速度设置", "工位一R轴运行速度", filePath));
	m_data.Vel[14] = DECIMAL(ReadConfigString("运行速度设置", "工位二X轴运行速度", filePath));
	m_data.Vel[15] = DECIMAL(ReadConfigString("运行速度设置", "工位二Y轴运行速度", filePath));
	m_data.Vel[16] = DECIMAL(ReadConfigString("运行速度设置", "工位二大负载Z1轴运行速度", filePath));
	m_data.Vel[17] = DECIMAL(ReadConfigString("运行速度设置", "工位二小负载Z2轴运行速度", filePath));
	m_data.Vel[18] = DECIMAL(ReadConfigString("运行速度设置", "工位二治具Z3轴运行速度", filePath));
	m_data.Vel[19] = DECIMAL(ReadConfigString("运行速度设置", "工位二R轴运行速度", filePath));
	m_data.Vel[20] = DECIMAL(ReadConfigString("运行速度设置", "后龙门X轴运行速度", filePath));
	m_data.Vel[21] = DECIMAL(ReadConfigString("运行速度设置", "后龙门Y轴运行速度", filePath));
	m_data.Vel[22] = DECIMAL(ReadConfigString("运行速度设置", "后龙门Z轴运行速度", filePath));
	m_data.Vel[23] = DECIMAL(ReadConfigString("运行速度设置", "后龙门料盒Y轴运行速度", filePath));


	//前龙门点位
	m_data.PointQ[0] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位1", "X轴", filePath));
	m_data.PointQ[1] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位1", "Y轴", filePath));
	m_data.PointQ[2] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位1", "Z轴", filePath));

	m_data.PointQ[3] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位2", "X轴", filePath));
	m_data.PointQ[4] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位2", "Y轴", filePath));
	m_data.PointQ[5] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位2", "Z轴", filePath));

	m_data.PointQ[6] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位3", "X轴", filePath));
	m_data.PointQ[7] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位3", "Y轴", filePath));
	m_data.PointQ[8] = DECIMAL(ReadConfigString("前龙门料盘拍照示教位3", "Z轴", filePath));

	m_data.PointQ[9] = DECIMAL(ReadConfigString("前龙门OK放料示教位1", "X轴", filePath));
	m_data.PointQ[10] = DECIMAL(ReadConfigString("前龙门OK放料示教位1", "Y轴", filePath));
	m_data.PointQ[11] = DECIMAL(ReadConfigString("前龙门OK放料示教位1", "Z轴", filePath));

	m_data.PointQ[12] = DECIMAL(ReadConfigString("前龙门OK放料示教位2", "X轴", filePath));
	m_data.PointQ[13] = DECIMAL(ReadConfigString("前龙门OK放料示教位2", "Y轴", filePath));
	m_data.PointQ[14] = DECIMAL(ReadConfigString("前龙门OK放料示教位2", "Z轴", filePath));

	m_data.PointQ[15] = DECIMAL(ReadConfigString("前龙门OK放料示教位3", "X轴", filePath));
	m_data.PointQ[16] = DECIMAL(ReadConfigString("前龙门OK放料示教位3", "Y轴", filePath));
	m_data.PointQ[17] = DECIMAL(ReadConfigString("前龙门OK放料示教位3", "Z轴", filePath));

	m_data.PointQ[19] = DECIMAL(ReadConfigString("前龙门NG放料示教位1", "X轴", filePath));
	m_data.PointQ[19] = DECIMAL(ReadConfigString("前龙门NG放料示教位1", "Y轴", filePath));
	m_data.PointQ[20] = DECIMAL(ReadConfigString("前龙门NG放料示教位1", "Z轴", filePath));

	m_data.PointQ[21] = DECIMAL(ReadConfigString("前龙门NG放料示教位2", "X轴", filePath));
	m_data.PointQ[22] = DECIMAL(ReadConfigString("前龙门NG放料示教位2", "Y轴", filePath));
	m_data.PointQ[23] = DECIMAL(ReadConfigString("前龙门NG放料示教位2", "Z轴", filePath));

	m_data.PointQ[24] = DECIMAL(ReadConfigString("前龙门NG放料示教位3", "X轴", filePath));
	m_data.PointQ[25] = DECIMAL(ReadConfigString("前龙门NG放料示教位3", "Y轴", filePath));
	m_data.PointQ[26] = DECIMAL(ReadConfigString("前龙门NG放料示教位3", "Z轴", filePath));

	m_data.PointQ[27] = DECIMAL(ReadConfigString("前龙门下相机拍照位", "X轴", filePath));
	m_data.PointQ[28] = DECIMAL(ReadConfigString("前龙门下相机拍照位", "Y轴", filePath));
	m_data.PointQ[29] = DECIMAL(ReadConfigString("前龙门下相机拍照位", "Z轴", filePath));

	m_data.PointQ[30] = DECIMAL(ReadConfigString("前龙门工位一下料位", "X轴", filePath));
	m_data.PointQ[31] = DECIMAL(ReadConfigString("前龙门工位一下料位", "Y轴", filePath));
	m_data.PointQ[32] = DECIMAL(ReadConfigString("前龙门工位一下料位", "Z轴", filePath));

	m_data.PointQ[33] = DECIMAL(ReadConfigString("前龙门工位一上料位", "X轴", filePath));
	m_data.PointQ[34] = DECIMAL(ReadConfigString("前龙门工位一上料位", "Y轴", filePath));
	m_data.PointQ[35] = DECIMAL(ReadConfigString("前龙门工位一上料位", "Z轴", filePath));

	m_data.PointQ[36] = DECIMAL(ReadConfigString("前龙门工位二下料位", "X轴", filePath));
	m_data.PointQ[37] = DECIMAL(ReadConfigString("前龙门工位二下料位", "Y轴", filePath));
	m_data.PointQ[38] = DECIMAL(ReadConfigString("前龙门工位二下料位", "Z轴", filePath));

	m_data.PointQ[39] = DECIMAL(ReadConfigString("前龙门工位二上料位", "X轴", filePath));
	m_data.PointQ[40] = DECIMAL(ReadConfigString("前龙门工位二上料位", "Y轴", filePath));
	m_data.PointQ[41] = DECIMAL(ReadConfigString("前龙门工位二上料位", "Z轴", filePath));

	m_data.PointQ[45] = DECIMAL(ReadConfigString("前龙门推杆夹料盘位", "X轴", filePath));
	m_data.PointQ[48] = DECIMAL(ReadConfigString("前龙门推杆料盘到位", "X轴", filePath));

	m_data.PointQ[58] = DECIMAL(ReadConfigString("前龙门料盘上下料位", "Y轴", filePath));

	m_data.PointQ[65] = DECIMAL(ReadConfigString("前龙门弹夹上下料位", "Z轴", filePath));
	m_data.PointQ[68] = DECIMAL(ReadConfigString("前龙门弹夹首层位置", "Z轴", filePath));
	m_data.PointQ[71] = DECIMAL(ReadConfigString("前龙门弹夹末层位置", "Z轴", filePath));

	//工位一点位设置
	m_data.PointG1[0] = DECIMAL(ReadConfigString("工位一Lens位置", "X轴", filePath));
	m_data.PointG1[1] = DECIMAL(ReadConfigString("工位一Lens位置", "Y轴", filePath));
	m_data.PointG1[2] = DECIMAL(ReadConfigString("工位一Lens位置", "Z1轴", filePath));
	m_data.PointG1[3] = DECIMAL(ReadConfigString("工位一Lens位置", "Z2轴", filePath));

	m_data.PointG1[4] = DECIMAL(ReadConfigString("工位一Cone位置", "X轴", filePath));
	m_data.PointG1[5] = DECIMAL(ReadConfigString("工位一Cone位置", "Y轴", filePath));
	m_data.PointG1[6] = DECIMAL(ReadConfigString("工位一Cone位置", "Z1轴", filePath));
	m_data.PointG1[7] = DECIMAL(ReadConfigString("工位一Cone位置", "Z2轴", filePath));

	m_data.PointG1[8] = DECIMAL(ReadConfigString("工位一拍照位置", "X轴", filePath));
	m_data.PointG1[9] = DECIMAL(ReadConfigString("工位一拍照位置", "Y轴", filePath));
	m_data.PointG1[10] = DECIMAL(ReadConfigString("工位一拍照位置", "Z1轴", filePath));
	m_data.PointG1[11] = DECIMAL(ReadConfigString("工位一拍照位置", "Z2轴", filePath));

	m_data.PointG1[12] = DECIMAL(ReadConfigString("工位一点胶拍照位置", "X轴", filePath));
	m_data.PointG1[13] = DECIMAL(ReadConfigString("工位一点胶拍照位置", "Y轴", filePath));
	m_data.PointG1[14] = DECIMAL(ReadConfigString("工位一点胶拍照位置", "Z1轴", filePath));
	m_data.PointG1[15] = DECIMAL(ReadConfigString("工位一点胶拍照位置", "Z2轴", filePath));

	m_data.PointG1[16] = DECIMAL(ReadConfigString("工位一对位视觉补偿位", "X轴", filePath));
	m_data.PointG1[17] = DECIMAL(ReadConfigString("工位一对位视觉补偿位", "Y轴", filePath));
	m_data.PointG1[18] = DECIMAL(ReadConfigString("工位一对位视觉补偿位", "Z1轴", filePath));
	m_data.PointG1[19] = DECIMAL(ReadConfigString("工位一对位视觉补偿位", "Z2轴", filePath));

	m_data.PointG1[20] = DECIMAL(ReadConfigString("工位一NG下料位", "X轴", filePath));
	m_data.PointG1[21] = DECIMAL(ReadConfigString("工位一NG下料位", "Y轴", filePath));
	m_data.PointG1[22] = DECIMAL(ReadConfigString("工位一NG下料位", "Z1轴", filePath));
	m_data.PointG1[23] = DECIMAL(ReadConfigString("工位一NG下料位", "Z2轴", filePath));

	m_data.PointG1[24] = DECIMAL(ReadConfigString("工位一手动直线插补", "X轴", filePath));
	m_data.PointG1[25] = DECIMAL(ReadConfigString("工位一手动直线插补", "Y轴", filePath));
	m_data.PointG1[26] = DECIMAL(ReadConfigString("工位一手动直线插补", "Z1轴", filePath));
	m_data.PointG1[27] = DECIMAL(ReadConfigString("工位一手动直线插补", "Z2轴", filePath));

	m_data.PointG1[40] = DECIMAL(ReadConfigString("工位一AA参数", "旋转R轴初始位", filePath));  //工位一AA参数
	m_data.PointG1[41] = DECIMAL(ReadConfigString("工位一AA参数", "旋转R轴步长值", filePath));
	m_data.PointG1[42] = DECIMAL(ReadConfigString("工位一AA参数", "治具Z3轴初始位", filePath));
	m_data.PointG1[43] = DECIMAL(ReadConfigString("工位一AA参数", "治具Z3轴步长值", filePath));
	m_data.PointG1[44] = DECIMAL(ReadConfigString("工位一AA参数", "X轴步长值", filePath));
	m_data.PointG1[45] = DECIMAL(ReadConfigString("工位一AA参数", "Y轴步长值", filePath));


	//工位二点位设置
	m_data.PointG2[0] = DECIMAL(ReadConfigString("工位二Lens位置", "X轴", filePath));
	m_data.PointG2[1] = DECIMAL(ReadConfigString("工位二Lens位置", "Y轴", filePath));
	m_data.PointG2[2] = DECIMAL(ReadConfigString("工位二Lens位置", "Z1轴", filePath));
	m_data.PointG2[3] = DECIMAL(ReadConfigString("工位二Lens位置", "Z2轴", filePath));

	m_data.PointG2[4] = DECIMAL(ReadConfigString("工位二Cone位置", "X轴", filePath));
	m_data.PointG2[5] = DECIMAL(ReadConfigString("工位二Cone位置", "Y轴", filePath));
	m_data.PointG2[6] = DECIMAL(ReadConfigString("工位二Cone位置", "Z1轴", filePath));
	m_data.PointG2[7] = DECIMAL(ReadConfigString("工位二Cone位置", "Z2轴", filePath));

	m_data.PointG2[8] = DECIMAL(ReadConfigString("工位二拍照位置", "X轴", filePath));
	m_data.PointG2[9] = DECIMAL(ReadConfigString("工位二拍照位置", "Y轴", filePath));
	m_data.PointG2[10] = DECIMAL(ReadConfigString("工位二拍照位置", "Z1轴", filePath));
	m_data.PointG2[11] = DECIMAL(ReadConfigString("工位二拍照位置", "Z2轴", filePath));

	m_data.PointG2[12] = DECIMAL(ReadConfigString("工位二点胶拍照位置", "X轴", filePath));
	m_data.PointG2[13] = DECIMAL(ReadConfigString("工位二点胶拍照位置", "Y轴", filePath));
	m_data.PointG2[14] = DECIMAL(ReadConfigString("工位二点胶拍照位置", "Z1轴", filePath));
	m_data.PointG2[15] = DECIMAL(ReadConfigString("工位二点胶拍照位置", "Z2轴", filePath));

	m_data.PointG2[16] = DECIMAL(ReadConfigString("工位二对位视觉补偿位", "X轴", filePath));
	m_data.PointG2[17] = DECIMAL(ReadConfigString("工位二对位视觉补偿位", "Y轴", filePath));
	m_data.PointG2[18] = DECIMAL(ReadConfigString("工位二对位视觉补偿位", "Z1轴", filePath));
	m_data.PointG2[19] = DECIMAL(ReadConfigString("工位二对位视觉补偿位", "Z2轴", filePath));

	m_data.PointG2[20] = DECIMAL(ReadConfigString("工位二NG下料位", "X轴", filePath));
	m_data.PointG2[21] = DECIMAL(ReadConfigString("工位二NG下料位", "Y轴", filePath));
	m_data.PointG2[22] = DECIMAL(ReadConfigString("工位二NG下料位", "Z1轴", filePath));
	m_data.PointG2[23] = DECIMAL(ReadConfigString("工位二NG下料位", "Z2轴", filePath));

	m_data.PointG2[24] = DECIMAL(ReadConfigString("工位二手动直线插补", "X轴", filePath));
	m_data.PointG2[25] = DECIMAL(ReadConfigString("工位二手动直线插补", "Y轴", filePath));
	m_data.PointG2[26] = DECIMAL(ReadConfigString("工位二手动直线插补", "Z1轴", filePath));
	m_data.PointG2[27] = DECIMAL(ReadConfigString("工位二手动直线插补", "Z2轴", filePath));

	m_data.PointG2[40] = DECIMAL(ReadConfigString("工位二AA参数", "旋转R轴初始位", filePath)); //工位二AA参数
	m_data.PointG2[41] = DECIMAL(ReadConfigString("工位二AA参数", "旋转R轴步长值", filePath));
	m_data.PointG2[42] = DECIMAL(ReadConfigString("工位二AA参数", "治具Z3轴初始位", filePath));
	m_data.PointG2[43] = DECIMAL(ReadConfigString("工位二AA参数", "治具Z3轴步长值", filePath));
	m_data.PointG2[44] = DECIMAL(ReadConfigString("工位二AA参数", "X轴步长值", filePath));
	m_data.PointG2[45] = DECIMAL(ReadConfigString("工位二AA参数", "Y轴步长值", filePath));


	//后龙门点位
	m_data.PointH[0] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位1", "X轴", filePath));
	m_data.PointH[1] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位1", "Y轴", filePath));
	m_data.PointH[2] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位1", "Z轴", filePath));

	m_data.PointH[3] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位2", "X轴", filePath));
	m_data.PointH[4] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位2", "Y轴", filePath));
	m_data.PointH[5] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位2", "Z轴", filePath));

	m_data.PointH[6] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位3", "X轴", filePath));
	m_data.PointH[7] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位3", "Y轴", filePath));
	m_data.PointH[8] = DECIMAL(ReadConfigString("后龙门料盘拍照示教位3", "Z轴", filePath));

	m_data.PointH[27] = DECIMAL(ReadConfigString("后龙门下相机拍照位", "X轴", filePath));
	m_data.PointH[28] = DECIMAL(ReadConfigString("后龙门下相机拍照位", "Y轴", filePath));
	m_data.PointH[29] = DECIMAL(ReadConfigString("后龙门下相机拍照位", "Z轴", filePath));

	m_data.PointH[30] = DECIMAL(ReadConfigString("后龙门工位一下料位", "X轴", filePath));
	m_data.PointH[31] = DECIMAL(ReadConfigString("后龙门工位一下料位", "Y轴", filePath));
	m_data.PointH[32] = DECIMAL(ReadConfigString("后龙门工位一下料位", "Z轴", filePath));

	m_data.PointH[33] = DECIMAL(ReadConfigString("后龙门工位一上料位", "X轴", filePath));
	m_data.PointH[34] = DECIMAL(ReadConfigString("后龙门工位一上料位", "Y轴", filePath));
	m_data.PointH[35] = DECIMAL(ReadConfigString("后龙门工位一上料位", "Z轴", filePath));

	m_data.PointH[36] = DECIMAL(ReadConfigString("后龙门工位二下料位", "X轴", filePath));
	m_data.PointH[37] = DECIMAL(ReadConfigString("后龙门工位二下料位", "Y轴", filePath));
	m_data.PointH[38] = DECIMAL(ReadConfigString("后龙门工位二下料位", "Z轴", filePath));

	m_data.PointH[39] = DECIMAL(ReadConfigString("后龙门工位二上料位", "X轴", filePath));
	m_data.PointH[40] = DECIMAL(ReadConfigString("后龙门工位二上料位", "Y轴", filePath));
	m_data.PointH[41] = DECIMAL(ReadConfigString("后龙门工位二上料位", "Z轴", filePath));

	m_data.PointH[58] = DECIMAL(ReadConfigString("后龙门料盘上下料位", "Y轴", filePath));


	//工位一电气比例阀参数
	m_data.AnalogPara[0] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z1吸嘴吸料盘", filePath));
	m_data.AnalogPara[1] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z1工位一上料", filePath));
	m_data.AnalogPara[2] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z1工位二上料", filePath));
	m_data.AnalogPara[3] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z1手动设置模拟值", filePath));

	m_data.AnalogPara[5] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z2吸嘴吸料盘", filePath));
	m_data.AnalogPara[6] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z2工位一上料", filePath));
	m_data.AnalogPara[7] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z2工位二上料", filePath));
	m_data.AnalogPara[8] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z2手动设置模拟值", filePath));

	m_data.AnalogPara[10] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z3吸嘴吸料盘", filePath));
	m_data.AnalogPara[11] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z3工位一上料", filePath));
	m_data.AnalogPara[12] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z3工位二上料", filePath));
	m_data.AnalogPara[13] = DECIMAL(ReadConfigString("前龙门电气比例阀参数", "Z3手动设置模拟值", filePath));

	m_data.AnalogPara[15] = DECIMAL(ReadConfigString("工位一电气比例阀参数", "小负载吸嘴", filePath));
	m_data.AnalogPara[16] = DECIMAL(ReadConfigString("工位一电气比例阀参数", "大负载AA前", filePath));
	m_data.AnalogPara[17] = DECIMAL(ReadConfigString("工位一电气比例阀参数", "大负载AA后", filePath));
	m_data.AnalogPara[18] = DECIMAL(ReadConfigString("工位一电气比例阀参数", "手动设置模拟值", filePath));

	m_data.AnalogPara[20] = DECIMAL(ReadConfigString("工位二电气比例阀参数", "小负载吸嘴", filePath));
	m_data.AnalogPara[21] = DECIMAL(ReadConfigString("工位二电气比例阀参数", "大负载AA前", filePath));
	m_data.AnalogPara[22] = DECIMAL(ReadConfigString("工位二电气比例阀参数", "大负载AA后", filePath));
	m_data.AnalogPara[23] = DECIMAL(ReadConfigString("工位二电气比例阀参数", "手动设置模拟值", filePath));

	m_data.AnalogPara[25] = DECIMAL(ReadConfigString("后龙门电气比例阀参数", "吸嘴吸料盘", filePath));
	m_data.AnalogPara[26] = DECIMAL(ReadConfigString("后龙门电气比例阀参数", "工位一上料", filePath));
	m_data.AnalogPara[27] = DECIMAL(ReadConfigString("后龙门电气比例阀参数", "工位二上料", filePath));
	m_data.AnalogPara[28] = DECIMAL(ReadConfigString("后龙门电气比例阀参数", "手动设置模拟值", filePath));
	*/
}

void LensAA::WriteAccTime()
{
// 	m_data.AccTime[0] = WIN_Q.Col_Acc0.Value;
// 	m_data.AccTime[1] = WIN_Q.Col_Acc1.Value;
// 	m_data.AccTime[2] = WIN_Q.Col_Acc2.Value;
// 	m_data.AccTime[3] = WIN_Q.Col_Acc3.Value;
// 	m_data.AccTime[4] = WIN_Q.Col_Acc4.Value;
// 	m_data.AccTime[5] = WIN_Q.Col_Acc5.Value;
// 	m_data.AccTime[6] = WIN_Q.Col_Acc6.Value;
// 	m_data.AccTime[7] = WIN_Q.Col_Acc7.Value;
// 	m_data.AccTime[8] = WIN_G1.Col_Acc8.Value;
// 	m_data.AccTime[9] = WIN_G1.Col_Acc9.Value;
// 	m_data.AccTime[10] = WIN_G1.Col_Acc10.Value;
// 	m_data.AccTime[11] = WIN_G1.Col_Acc11.Value;
// 	m_data.AccTime[12] = WIN_G1.Col_Acc12.Value;
// 	m_data.AccTime[13] = WIN_G1.Col_Acc13.Value;
// 	m_data.AccTime[14] = WIN_G2.Col_Acc14.Value;
// 	m_data.AccTime[15] = WIN_G2.Col_Acc15.Value;
// 	m_data.AccTime[16] = WIN_G2.Col_Acc16.Value;
// 	m_data.AccTime[17] = WIN_G2.Col_Acc17.Value;
// 	m_data.AccTime[18] = WIN_G2.Col_Acc18.Value;
// 	m_data.AccTime[19] = WIN_G2.Col_Acc19.Value;
// 	m_data.AccTime[20] = WIN_H.Col_Acc20.Value;
// 	m_data.AccTime[21] = WIN_H.Col_Acc21.Value;
// 	m_data.AccTime[22] = WIN_H.Col_Acc22.Value;
// 	m_data.AccTime[23] = WIN_H.Col_Acc23.Value;
// 
// 
// 
// 	m_data.AccTime[28] = WIN_G1.Col_Acc28.Value;
// 	m_data.AccTime[29] = WIN_G2.Col_Acc29.Value;
// 
// 	DoubleSend(128, m_data.AccTime);
}

void LensAA::WriteDecTime()
{
// 	m_data.DecTime[0] = WIN_Q.Col_Dec0.Value;
// 	m_data.DecTime[1] = WIN_Q.Col_Dec1.Value;
// 	m_data.DecTime[2] = WIN_Q.Col_Dec2.Value;
// 	m_data.DecTime[3] = WIN_Q.Col_Dec3.Value;
// 	m_data.DecTime[4] = WIN_Q.Col_Dec4.Value;
// 	m_data.DecTime[5] = WIN_Q.Col_Dec5.Value;
// 	m_data.DecTime[6] = WIN_Q.Col_Dec6.Value;
// 	m_data.DecTime[7] = WIN_Q.Col_Dec7.Value;
// 	m_data.DecTime[8] = WIN_G1.Col_Dec8.Value;
// 	m_data.DecTime[9] = WIN_G1.Col_Dec9.Value;
// 	m_data.DecTime[10] = WIN_G1.Col_Dec10.Value;
// 	m_data.DecTime[11] = WIN_G1.Col_Dec11.Value;
// 	m_data.DecTime[12] = WIN_G1.Col_Dec12.Value;
// 	m_data.DecTime[13] = WIN_G1.Col_Dec13.Value;
// 	m_data.DecTime[14] = WIN_G2.Col_Dec14.Value;
// 	m_data.DecTime[15] = WIN_G2.Col_Dec15.Value;
// 	m_data.DecTime[16] = WIN_G2.Col_Dec16.Value;
// 	m_data.DecTime[17] = WIN_G2.Col_Dec17.Value;
// 	m_data.DecTime[18] = WIN_G2.Col_Dec18.Value;
// 	m_data.DecTime[19] = WIN_G2.Col_Dec19.Value;
// 	m_data.DecTime[20] = WIN_H.Col_Dec20.Value;
// 	m_data.DecTime[21] = WIN_H.Col_Dec21.Value;
// 	m_data.DecTime[22] = WIN_H.Col_Dec22.Value;
// 	m_data.DecTime[23] = WIN_H.Col_Dec23.Value;
// 
// 
// 
// 	m_data.DecTime[28] = WIN_G1.Col_Dec28.Value;
// 	m_data.DecTime[29] = WIN_G2.Col_Dec29.Value;
// 
// 
// 	DoubleSend(192, m_data.DecTime);
}

void LensAA::WriteJogVel()
{
// 	m_data.JogVel[0] = WIN_Q.Col_JogVel0.Value;
// 	m_data.JogVel[1] = WIN_Q.Col_JogVel1.Value;
// 	m_data.JogVel[2] = WIN_Q.Col_JogVel2.Value;
// 	m_data.JogVel[3] = WIN_Q.Col_JogVel3.Value;
// 	m_data.JogVel[4] = WIN_Q.Col_JogVel4.Value;
// 	m_data.JogVel[5] = WIN_Q.Col_JogVel5.Value;
// 	m_data.JogVel[6] = WIN_Q.Col_JogVel6.Value;
// 	m_data.JogVel[7] = WIN_Q.Col_JogVel7.Value;
// 	m_data.JogVel[8] = WIN_G1.Col_JogVel8.Value;
// 	m_data.JogVel[9] = WIN_G1.Col_JogVel9.Value;
// 	m_data.JogVel[10] = WIN_G1.Col_JogVel10.Value;
// 	m_data.JogVel[11] = WIN_G1.Col_JogVel11.Value;
// 	m_data.JogVel[12] = WIN_G1.Col_JogVel12.Value;
// 	m_data.JogVel[13] = WIN_G1.Col_JogVel13.Value;
// 	m_data.JogVel[14] = WIN_G2.Col_JogVel14.Value;
// 	m_data.JogVel[15] = WIN_G2.Col_JogVel15.Value;
// 	m_data.JogVel[16] = WIN_G2.Col_JogVel16.Value;
// 	m_data.JogVel[17] = WIN_G2.Col_JogVel17.Value;
// 	m_data.JogVel[18] = WIN_G2.Col_JogVel18.Value;
// 	m_data.JogVel[19] = WIN_G2.Col_JogVel19.Value;
// 	m_data.JogVel[20] = WIN_H.Col_JogVel20.Value;
// 	m_data.JogVel[21] = WIN_H.Col_JogVel21.Value;
// 	m_data.JogVel[22] = WIN_H.Col_JogVel22.Value;
// 	m_data.JogVel[23] = WIN_H.Col_JogVel23.Value;
// 
// 	DoubleSend(256, m_data.JogVel);
}

void LensAA::WriteVel()
{
// 	m_data.Vel[0] = WIN_Q.Col_Vel0.Value;
// 	m_data.Vel[1] = WIN_Q.Col_Vel1.Value;
// 	m_data.Vel[2] = WIN_Q.Col_Vel2.Value;
// 	m_data.Vel[3] = WIN_Q.Col_Vel3.Value;
// 	m_data.Vel[4] = WIN_Q.Col_Vel4.Value;
// 	m_data.Vel[5] = WIN_Q.Col_Vel5.Value;
// 	m_data.Vel[6] = WIN_Q.Col_Vel6.Value;
// 	m_data.Vel[7] = WIN_Q.Col_Vel7.Value;
// 	m_data.Vel[8] = WIN_G1.Col_Vel8.Value;
// 	m_data.Vel[9] = WIN_G1.Col_Vel9.Value;
// 	m_data.Vel[10] = WIN_G1.Col_Vel10.Value;
// 	m_data.Vel[11] = WIN_G1.Col_Vel11.Value;
// 	m_data.Vel[12] = WIN_G1.Col_Vel12.Value;
// 	m_data.Vel[13] = WIN_G1.Col_Vel13.Value;
// 	m_data.Vel[14] = WIN_G2.Col_Vel14.Value;
// 	m_data.Vel[15] = WIN_G2.Col_Vel15.Value;
// 	m_data.Vel[16] = WIN_G2.Col_Vel16.Value;
// 	m_data.Vel[17] = WIN_G2.Col_Vel17.Value;
// 	m_data.Vel[18] = WIN_G2.Col_Vel18.Value;
// 	m_data.Vel[19] = WIN_G2.Col_Vel19.Value;
// 	m_data.Vel[20] = WIN_H.Col_Vel20.Value;
// 	m_data.Vel[21] = WIN_H.Col_Vel21.Value;
// 	m_data.Vel[22] = WIN_H.Col_Vel22.Value;
// 	m_data.Vel[23] = WIN_H.Col_Vel23.Value;
// 
// 
// 	m_data.Vel[28] = WIN_G1.Col_Vel28.Value;
// 	m_data.Vel[29] = WIN_G2.Col_Vel29.Value;
// 
// 	DoubleSend(320, m_data.Vel);
}

void LensAA::WritePos()
{
// 	m_data.Pos[0] = WIN_Q.Col_Pos0.Value;
// 	m_data.Pos[1] = WIN_Q.Col_Pos1.Value;
// 	m_data.Pos[2] = WIN_Q.Col_Pos2.Value;
// 	m_data.Pos[3] = WIN_Q.Col_Pos3.Value;
// 	m_data.Pos[4] = WIN_Q.Col_Pos4.Value;
// 	m_data.Pos[5] = WIN_Q.Col_Pos5.Value;
// 	m_data.Pos[6] = WIN_Q.Col_Pos6.Value;
// 	m_data.Pos[7] = WIN_Q.Col_Pos7.Value;
// 	m_data.Pos[8] = WIN_G1.Col_Pos8.Value;
// 	m_data.Pos[9] = WIN_G1.Col_Pos9.Value;
// 	m_data.Pos[10] = WIN_G1.Col_Pos10.Value;
// 	m_data.Pos[11] = WIN_G1.Col_Pos11.Value;
// 	m_data.Pos[12] = WIN_G1.Col_Pos12.Value;
// 	m_data.Pos[13] = WIN_G1.Col_Pos13.Value;
// 	m_data.Pos[14] = WIN_G2.Col_Pos14.Value;
// 	m_data.Pos[15] = WIN_G2.Col_Pos15.Value;
// 	m_data.Pos[16] = WIN_G2.Col_Pos16.Value;
// 	m_data.Pos[17] = WIN_G2.Col_Pos17.Value;
// 	m_data.Pos[18] = WIN_G2.Col_Pos18.Value;
// 	m_data.Pos[19] = WIN_G2.Col_Pos19.Value;
// 	m_data.Pos[20] = WIN_H.Col_Pos20.Value;
// 	m_data.Pos[21] = WIN_H.Col_Pos21.Value;
// 	m_data.Pos[22] = WIN_H.Col_Pos22.Value;
// 	m_data.Pos[23] = WIN_H.Col_Pos23.Value;
// 
// 	DoubleSend(384, m_data.Pos);
}

void LensAA::WriteConfigString(wchar_t* section, wchar_t* key, wchar_t* val, wchar_t* filePath)
{
	WritePrivateProfileString(section, key, val, filePath);
}

wchar_t* LensAA::ReadConfigString(wchar_t* Section, wchar_t* KeyName, wchar_t* defVal, wchar_t* Path)
{
	static wchar_t buf[256];
	memset(buf, 0, 256 * sizeof(wchar_t));
	GetPrivateProfileString(Section, KeyName, defVal, buf, 256, Path);
	return buf;
}

void LensAA::SetAxisID(int axisID)
{//只操作工位一个的六个轴，所以从LAN_Station1_X开始
	m_axis += LAN_Station1_X;
}