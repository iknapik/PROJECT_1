// project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "StudentInfo.h"
#include "StudentDao.h"
#include "StudentModel.h"
#include "IdManager.h"
#include "CSVDb.h"
#include <cassert>
#include <array>
#include <cstdio>

using namespace school;
using namespace cheshire;
bool operator<(const std::unique_ptr<StudentInfo>& s1, const std::unique_ptr<StudentInfo>& s2)
{
	std::cout << "hello";
	return s1->get_id() < s2->get_id();
}



int main()
{
	bool test_csvdb = false;
	if (test_csvdb)
	{
		auto csv = CSVDb<5>("test_db.txt");
		std::remove("test_db.txt");
		std::array<std::string, 5> m1{ "103", "Tomek", "Kowalski", "3123123", "Inwałd Zielna 20" };
		std::array<std::string, 5> m2{ "50", "Tomek", "Kowalski2", "3123123", "Inwałd Zielna 20" };
		std::array<std::string, 5> m3{ "60", "Tomek", "Kowalski3", "3123123", "Inwałd Zielna 20" };
		for (int i = 1; i < 100; ++i)
		{
			m1[0] = std::to_string(i);
			csv.add_row(m1);
		}
		csv.update_row(m2);
		csv.update_row(m3);
		assert(m2[2] == csv.get_row(50)[2]);
		assert(m3[2] == csv.get_row(60)[2]);
		auto rows = csv.get_rows();
		for (auto &i : *rows)
		{
			std::cout << (*i)[0] << " " << (*i)[1] << " " << (*i)[2] << " " << (*i)[3] << " " << (*i)[4] << "\n";
		}
	}
	bool test_studentdao = true;
	if (test_studentdao)
	{
		auto sda = StudentDao();
		std::remove("students_db.txt");
		for (int i = 1 ;i < 100; ++i)
		{
			auto sinfo1 = StudentInfo("Michal", "Kowalski", "3123213", "Inwałd", "Zielna 19", i);
			sda.add_student(sinfo1);
		}
		auto sinfo1 = StudentInfo("Michal", "Kowalski1", "3123213", "Inwałd", "Zielna 19", 30);
		auto sinfo2 = StudentInfo("Michal", "Kowalski2", "3123213", "Inwałd", "Zielna 19", 70);
		auto sinfo3 = StudentInfo("Michal", "Kowalski3", "3123213", "Inwałd", "Zielna 19", 80);
		sda.update_student(sinfo1);
		sda.update_student(sinfo2);
		sda.update_student(sinfo3);
		sda.remove_student(14);
		sda.remove_student(20);
		assert(sinfo1.m_lastname == sda.get_student(30)->m_lastname);
		assert(sinfo2.m_lastname == sda.get_student(70)->m_lastname);
		assert(sinfo3.m_lastname == sda.get_student(80)->m_lastname);
		auto map_ptr = sda.get_students();
		for (auto &i : *map_ptr)
		{
			std::cout << *i.second << "\n";
		}
	}
	return 0;
}