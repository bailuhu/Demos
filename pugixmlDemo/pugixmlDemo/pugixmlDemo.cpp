// pugixmlDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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

using namespace std;

// ����XML��
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

		pugi::xml_node xage = xstudent.append_child("Age");
		char buf[128] = { 0 };
		printf(buf, "%d", student.age);
		xage.append_child(pugi::node_pcdata).set_value(buf);
	}

	xdoc.print(std::cout);

	xdoc.save_file(kFileName);
}

// ��ȡ������XML��
static void ReadData() {
	std::list<Student> students;

	pugi::xml_document xdoc;
	if (!xdoc.load_file(kFileName)) {
		return;
	}

	pugi::xml_node xstudents = xdoc.child("Students");
	for (pugi::xml_node xstudent = xstudents.first_child(); xstudent != NULL; xstudent = xstudent.next_sibling()) {
		int id = xstudent.attribute("id").as_int();

		pugi::xml_node xname = xstudent.child("Name");
		std::string name = xname.text().as_string();

		pugi::xml_node xage = xstudent.child("Age");
		int age = xage.text().as_int();

		students.push_back(Student(id, name, age));
	}

	for (const Student& student : students) {
		student.Print();
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	WriteData();
	ReadData();
	return 0;
}

