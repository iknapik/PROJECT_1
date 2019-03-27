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
bool operator<(const std::unique_ptr<StudentInfo>& s1, const std::unique_ptr<StudentInfo>& s2)
{
	std::cout << "hello";
	return s1->get_id() < s2->get_id();
}



int main()
{
	bool test_csvdb = true;
	if (test_csvdb)
	{/*
		auto csv = CSVDb<5>("test_db.txt");
		std::cout << "Start test_csvdb.\n";
		auto timer = Timer();
		
		std::array<std::string, 5> m1{ "103", "Tomek", "Kowalski", "3123123", "Inwałd Zielna 20" };
		std::array<std::string, 5> m2{ "50", "Tomek", "Kowalski2", "3123123", "Inwałd Zielna 20" };
		std::array<std::string, 5> m3{ "60", "Tomek", "Kowalski3", "3123123", "Inwałd Zielna 20" };
		for (int i = 1; i < 1000; ++i)
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
		timer.elapsed();*/
		auto timer = Timer();
		std::remove("test_db.txt");
		auto csv = CSVDb("test_db.txt", {"Imie", "Nazwisko", "PESEL", "Miasto", "Adres"});
		auto m1 = std::map<const std::string, std::string>();
		m1["Imie"] = "Marek";
		m1["Nazwisko"] = "Kowalski";
		m1["PESEL"] = "312312312";
		m1["Miasto"] = "Inwałd";
		m1["Adres"] = "Zielna 19";
		for (int i = 1; i < 1000; ++i)	csv.add_row(i, m1);
		m1["Imie"] = "Tomek";
		csv.update_row(90, m1);
		m1["Imie"] = "Wojtek";
		csv.update_row(95, m1);
		m1["Imie"] = "Ala";
		//csv.update_row(99, m1);
		//assert("Ala" == csv.get_row(99).second->at("Imie"));
		assert("Tomek" == csv.get_row(90).second->at("Imie"));
		assert("Wojtek" == csv.get_row(95).second->at("Imie"));
		auto rows = csv.get_rows();
		for (auto& it : *rows)
		{
			std::cout << it.first << " " << it.second->at("Imie") << " " << it.second->at("Nazwisko") << "\n";
		}
		timer.elapsed();
		
	}
	bool test_studentdao = false;
	
	return 0;
}