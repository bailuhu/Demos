#pragma once


#include "TcpClientManager.h"

enum LensAA_Axis_Num
{
	// front door loading and unloading axis
	LAN_Front_X,
	LAN_Front_Y,
	LAN_Front_Z1,
	LAN_Front_Z2,
	LAN_Front_Z3,
	LAN_Front_T_X,
	LAN_Front_L_Y,
	LAN_Front_L_Z,
	// station 1 axis
	LAN_Station1_X = 8,
	LAN_Station1_Y,
	LAN_Station1_Z1,
	LAN_Station1_Z2,
	LAN_Station1_Z3,
	LAN_Station1_W, // U V W == Tx Ty Tz
	// station 2 axis
	LAN_Station2_X,
	LAN_Station2_Y,
	LAN_Station2_Z1,
	LAN_Station2_Z2,
	LAN_Station2_Z3,
	LAN_Station2_W, // U V W == Tx Ty Tz
	// read door loading and unloading axis
	LAN_Rear_X,
	LAN_Rear_Y,
	LAN_Rear_Z,
	LAN_Rear_L_Y,
};


struct UserData
{
	int Enable;
	int Home;
	int Idle;
	int LP;
	int LN;
	int RunStatus;
	int AlarmData[32]; //报警信息，每个字储存32个报警信息
	int AlarmID[32 * 32];

	DECIMAL AccTime;
	DECIMAL DecTime;
	DECIMAL JogVel;
	DECIMAL Vel;
	int Pos;
	DECIMAL ActPos;

	DECIMAL PointQ[100];  //前龙门点位数据
	DECIMAL PointG1[100];  //工位一点位数据
	DECIMAL PointG2[100];  //工位二点位数据
	DECIMAL PointH[100];  //后龙门点位数据

	DECIMAL CircPointG1[32];   //工位一圆弧插补坐标数据：起点、终点、中心点
	DECIMAL CircPointG2[32];   //工位一圆弧插补坐标数据

	DECIMAL AnalogPara[32];  //模拟量设定值

};

class LensAA
{
public:
	LensAA();
	~LensAA();

	TcpClientManager m_tcpClient;
	unsigned char m_client[4];
	unsigned char m_server[4];
	bool finsConnected;

	int OpenFins(wstring ip, int port);
	void CloseFins();

	void WriteData(int startAddr, int number, int type, unsigned char* data);
	unsigned char* ReadData(int startAddr, int number, int type);

	bool SendDword(int startAddr, int contWord);
	bool DintSend(int startAddr, int dIntData);
	bool DoubleSend(int startAddr, int axisID, int pos, int len);
	int RecvDword(int startAddr);

	void GetBytes(int data, unsigned char* dst);
	int ToInt32(unsigned char* src, int index);
	int m_axis;
	void SetAxisID(int axisID);

	bool Jog(int LensAA_Axis_Num, bool bPositive);

	UserData m_data;
	// bit operation
	int EnableBit(int src, int index);
	int DisableBit(int src, int index);
	int ReverseBit(int src, int index);
	int GetBit(int src, int index);

	// Load and save option
	void LoadOption();
	void SaveOption();
	void WriteConfigString(wchar_t* section, wchar_t* key, wchar_t* val, wchar_t* filePath);
	wchar_t* ReadConfigString(wchar_t* Section, wchar_t* KeyName, wchar_t* defVal, wchar_t* Path);

	// modify data
	void WriteOption();
	void ReadOption();

private:
	void WriteAccTime();
	void WriteDecTime();
	void WriteJogVel();
	void WriteVel();
	void WritePos();
};

