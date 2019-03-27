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


int main()
{
	bool test_csvdb = true;
	if (test_csvdb)
	{
		auto timer = Timer();
		std::remove("test_db.txt");
		auto csv = CSVDb("test_db.txt", {"Imie", "Nazwisko", "PESEL", "Miasto", "Adres"});
		auto m1 = std::map<const std::string, std::string>();
		m1["Imie"] = "Marek";
		m1["Nazwisko"] = "Kowalski";
		m1["PESEL"] = "312312312";
		m1["Miasto"] = "Inwałd";
		m1["Adres"] = "Zielna 19";
		for (int i = 1; i < 1000; ++i)
		{
			csv.add_row(i, m1);
			m1["PESEL"] += std::to_string(i);
		}
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