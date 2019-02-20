// pugixmlDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "pugixml.hpp"

static const char* kFileName = "students2.xml";

struct Student {
	int id;
	std::string name;
	int age;

	Student(int id_, const std::string& name_, int age_)
		: id(id_)
		, name(name_)
		, age(age_) {
	}

	void Print() const {
		std::cout << "Id: " << id << ", Name: " << name << ", Age: " << age << std::endl;
	}
};

#pragma pack(push, 1)   // 内存中对齐方式(1字节) 需与测试软件定义一致
typedef struct
{
	int  CmdID;			// 指令 1~99为测试项目ID，100~199启动、关闭模组、调焦命令，200~300获取测试项目信息
	int  Status;		// 回应请求：0执行请求，-1不执行请求
	int  Result;		// 回应结果:0 OK, 1 NG, -1 继续
	char ErrInfo[256];  // 附加信息字符串
	char TestData[256]; // 测试数据
}SockMsg;
#pragma pack(pop)

using namespace std;

void SetMsg(SockMsg* pMsg, int CmdID, int Status, int Result, string ErrInfo, string TestData, std::list<SockMsg>& ListSockMsg)
{
	pMsg->CmdID = CmdID;
	pMsg->Status = Status;
	pMsg->Result = Result;
// 	pMsg->ErrInfo = ErrInfo.c_str();
// 	pMsg->TestData = TestData.c_str();
	memcpy_s(pMsg->ErrInfo, 256, ErrInfo.c_str(), ErrInfo.length()+1);
	memcpy_s(pMsg->TestData, 256, TestData.c_str(), TestData.length()+1);

	
	ListSockMsg.push_back(*pMsg);
}

void WriteListSockMsg(std::list<SockMsg>& listSockMsg)
{
	pugi::xml_document xdoc;
	pugi::xml_node xdec = xdoc.prepend_child(pugi::node_declaration);
	xdec.append_attribute("version").set_value("1.0");
	xdec.append_attribute("encoding").set_value("utf-8");

	pugi::xml_node xstudents = xdoc.append_child("ListSockMsg");
	xstudents.append_child(pugi::node_comment).set_value("ListSockMsg Comment");
	for (const SockMsg& msg : listSockMsg)
	{
		pugi::xml_node xstudent = xstudents.append_child("MsgName");

		// add attribute
		pugi::xml_attribute xstudent_TestData = xstudent.append_attribute("TestData");
		xstudent_TestData.set_value(msg.TestData);
		pugi::xml_attribute xstudent_ErrInfo = xstudent.append_attribute("ErrInfo");
		xstudent_ErrInfo.set_value(msg.ErrInfo);
		pugi::xml_attribute xstudent_Result = xstudent.append_attribute("Result");
		xstudent_Result.set_value(msg.Result);
		pugi::xml_attribute xstudent_Status = xstudent.append_attribute("Status");
		xstudent_Status.set_value(msg.Status);
		pugi::xml_attribute xstudent_CmdID = xstudent.append_attribute("CmdID");
		xstudent_CmdID.set_value(msg.CmdID);

		// add child
// 		char temp[256] = { 0 };
// 		pugi::xml_node xname = xstudents.append_child("CmdID");
// 		sprintf_s(temp, 256, "%d", msg.CmdID);
// 		xname.append_child(pugi::node_pcdata).set_value(temp);
// 		xname = xstudents.append_child("Status");
// 		sprintf_s(temp, 256, "%d", msg.Status);
// 		xname.append_child(pugi::node_pcdata).set_value(temp);
// 		xname = xstudents.append_child("Result");
// 		sprintf_s(temp, 256, "%d", msg.Status);
// 		xname.append_child(pugi::node_pcdata).set_value(temp);
// 		xname = xstudents.append_child("ErrInfo");
// 		xname.append_child(pugi::node_pcdata).set_value(msg.ErrInfo);
// 		xname = xstudents.append_child("TestData");
// 		xname.append_child(pugi::node_pcdata).set_value(msg.TestData);
		
	}
	xdoc.print(std::cout);
	xdoc.save_file(kFileName);
}

void ReadListSockMsg(std::list<SockMsg>& listSockMsg)
{
	pugi::xml_document xdoc;
	if (!xdoc.load_file(kFileName)) {
		return;
	}

	pugi::xml_node xstudents = xdoc.child("ListSockMsg");
	for (pugi::xml_node xstudent = xstudents.first_child(); xstudent != NULL; xstudent = xstudent.next_sibling()) 
	{
		SockMsg tempMsg;
		tempMsg.CmdID = xstudent.attribute("CmdID").as_int();

		tempMsg.Status = xstudent.attribute("Status").as_int();

		tempMsg.Result = xstudent.attribute("Result").as_int();

		string szErrorInfo = xstudent.attribute("ErrInfo").as_string();
		memcpy_s(tempMsg.ErrInfo, 256, szErrorInfo.c_str(), szErrorInfo.length() + 1);

		string szTestData = xstudent.attribute("TestData").as_string();
		memcpy_s(tempMsg.TestData, 256, szTestData.c_str(), szTestData.length() + 1);

		listSockMsg.push_back(tempMsg);
	}

	for (const SockMsg& msg : listSockMsg) 
	{
		std::cout << msg.CmdID << "/" << msg.Status << "/" << msg.Result << "/" << msg.ErrInfo << "/" <<msg.TestData << std::endl;
	}
}

// 创建XML。
static void WriteData() {
	// Prepare data.
	std::list<Student> students;
	students.push_back(Student(1, "MaZhe", 18));
	students.push_back(Student(2, "Miki", 19));
	students.push_back(Student(3, "Maki", 20));

	// Serialization.
	pugi::xml_document xdoc;

	pugi::xml_node xdec = xdoc.prepend_child(pugi::node_declaration);
	xdec.append_attribute("version").set_value("1.0");
	xdec.append_attribute("encoding").set_value("utf-8");

	pugi::xml_node xstudents = xdoc.append_child("Students");
	xstudents.append_child(pugi::node_comment).set_value("Students!");

	for (const Student& student : students) {
		pugi::xml_node xstudent = xstudents.append_child("Student");

		pugi::xml_attribute xstudent_id = xstudent.append_attribute("id");
		xstudent_id.set_value(student.id);

		pugi::xml_node xname = xstudent.append_child("Name");
		xname.append_child(pugi::node_pcdata).set_value(student.name.c_str());

		pugi::xml_attribute xage = xstudent.append_attribute("Age");
		xage.set_value(student.age);
	}

	xdoc.print(std::cout);

	xdoc.save_file(kFileName);
}

// 读取并解析XML。
static void ReadData() {
	std::list<Student> students;

	pugi::xml_document xdoc;
	if (!xdoc.load_file(kFileName)) {
		return;
	}

	pugi::xml_node xstudents = xdoc.child("Students");
	for (pugi::xml_node xstudent = xstudents.first_child(); xstudent != NULL; xstudent = xstudent.next_sibling()) 
	{
		int id = xstudent.attribute("id").as_int();

		std::string name = xstudent.attribute("Name").as_string();

		int age = xstudent.attribute("Age").as_int();

		students.push_back(Student(id, name, age));
	}

	for (const Student& student : students) {
		student.Print();
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::list<SockMsg> g_ListSockMsg;
	SockMsg sendmsg;
	SetMsg(&sendmsg, 101, -1, -1, "error101", "test data101", g_ListSockMsg);
	SetMsg(&sendmsg, 1, 11, -1, "error001", "test data001", g_ListSockMsg);
	SetMsg(&sendmsg, 102, -1, -1, "error102", "test data102", g_ListSockMsg);
	WriteListSockMsg(g_ListSockMsg);
	g_ListSockMsg.clear();
	ReadListSockMsg(g_ListSockMsg);
// 	WriteData();
// 	ReadData();
	return 0;
}

