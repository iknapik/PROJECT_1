// project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DatabaseModel.h"
#include <cassert>
#include <array>
#include <cstdio>
#include <chrono>


class Timer
{
	std::chrono::time_point<std::chrono::system_clock> start;
public:
	Timer(bool start_message = true)
	{
		start = std::chrono::system_clock::now();
		if (start_message) std::cout << "Timer Starts!\n";
	}
	void elapsed()
	{
		auto now = std::chrono::system_clock::now();
		auto ticks = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
		std::cout << "Took: " << ticks / 1000 << "." << ticks - ticks / 1000<< " seconds.\n";
	}
};

using namespace school;
using namespace cheshire;


int main()
{
	bool test_csvdb = false;
	if (test_csvdb)
	{
		auto timer = Timer();
		std::remove("test_db.txt");
		std::remove("id_test_db.txt");
		auto csv = CSVDbID("test_db.txt", {"Imie", "Nazwisko", "PESEL", "Miasto", "Adres"});
		std::vector<std::string> m1{"Michał", "Kowalski", /*"3123123", "Inwałd", "Zielna 19"*/};
		for (int i = 1; i < 100; ++i)
		{
			assert(csv.add_row(m1));
			m1[1] = "Kowal" + std::to_string(i);
			
		}
		m1[0] = "Tomek";
		assert(csv.update_row(90, m1));
		m1[0] = "Wojtek";
		csv.update_row(95, m1);
		m1[0] = "Ala";
		assert(csv.update_row(99, m1));
		assert("Ala" == csv.get_row(99).second->at("Imie"));
		assert("Tomek" == csv.get_row(90).second->at("Imie"));
		assert("Wojtek" == csv.get_row(95).second->at("Imie"));
		assert(!csv.remove_row(101));
		assert(!csv.update_row(200, m1));
		assert(csv.remove_row(5));
		assert(csv.remove_row(10));
		assert(csv.remove_row(15));
		assert(csv.add_row(m1));
		assert(csv.add_row(m1));
		assert(csv.add_row(m1));
		assert(csv.get_row(15).first == 15);
		assert(csv.get_row(10).first == 10);
		assert(csv.get_row(5).first == 5);
		auto rows = csv.get_rows();
		for (auto& it : *rows)
		{
			std::cout << it.first << " " << it.second->at("Imie") << " " << it.second->at("Nazwisko") << "\n";
		}
		timer.elapsed();
	}
	
	std::cout << sizeof(CSVDb) << "csvdb\n";
	std::cout << sizeof(IdManager) << "IdManager\n";
	std::cout << sizeof(StudentInfo) << "StudentInfo\n";
	std::cout << sizeof(BasicData) << "BasicData\n";
		
	bool test_student_dao = true;
	
	if (test_student_dao)
	{
		std::remove("students_db.txt");
		std::remove("id_students_db.txt");
		auto timer = Timer();
		auto sdao = BaseDao<StudentInfo>("students_db.txt", school::STUDENT_FIELD_NAMES);
		auto m1 = StudentInfo("Kamil", "Kowalski", "9819232", "Inwałd", "ZIelan 19");
		for (int i = 0; i < 1000; ++i)
		{
			assert(sdao.add_row(m1));
			m1.m_firstname = "Kamil" + std::to_string(i);
		}
		m1.m_firstname = "Wojtek";
		assert(!sdao.update_row(m1));

		auto studs = sdao.get_rows();
		for (auto & pair : *studs)
		{
			std::cout << *pair.second << "\n";
		}
		timer.elapsed();
	}

	bool test_class_dao = false;
	if (test_class_dao)
	{
		std::remove("classes_db.txt");
		std::remove("id_classes_db.txt");
		auto timer = Timer();
		auto sdao = BaseDao<ClassInfo>("classes_db.txt", school::CLASS_FIELD_NAMES);
		auto m1 = ClassInfo(3, { 1, 2, 3, 4, 5, 6, 7 });
		sdao.add_row(m1);
		auto m1_back = sdao.get_row(1);
		std::cout << *m1_back;
	}
	
	bool test_model = true;
	{
		if (test_model)
		{
			Timer timer{};
			auto model = DatabaseModel();
			std::cout << "Database Initialized! \n";
			timer.elapsed();
			std::cout << "Enter Student Name: \n";
			std::string str;
			std::getline(std::cin, str);
			for (auto& i : model.get_students_by_ids({100, 200, 300, 7657, 2131, 4091}))
			{
				std::cout << "\n" << i;
			}
		}
	}
	return 0;
}