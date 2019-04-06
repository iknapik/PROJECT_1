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
	unsigned elapsed()
	{
		auto now = std::chrono::system_clock::now();
		auto ticks = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
		std::cout << "Took: " << ticks / 1000 << "." << ticks - ticks / 1000<< " seconds.\n";
		return ticks - ticks / 1000;
	}
};

using namespace school;
using namespace cheshire;


int main()
{
	std::remove("students_db.txt");
	std::remove("id_students_db.txt");
	Timer timer{};
	DatabaseModel db{};
	StudentInfo stud1{"Michal", "Kowalski", "9123819", "Wadowice", "Wiejska 3", 1};
	ClassInfo class1{"16i", 3};
	ProfessorInfo prof1{"Tomasz", "Wozniak", "Mgr", {"MD", "PE"}};
	MarkInfo mark1{4, MARK::_3, "04-01-2019", SUBJECT::MD, 3};
	//db.add(mark1);
	//db.add(stud1);
	//db.add(class1);
	//db.add(prof1);

	for (int i = 1; i < 1000; ++i)
	{
		db.add(stud1);
		stud1.m_firstname = "Michal" + std::to_string(i);
		stud1.m_class_id = i % 3;
	}

	std::string str = "16i";
	//std::getline(std::cin, str);
	for (auto &stud : db.get_students_by_class(str))
	{
		std::cout << stud << "\n";
	}
	timer.elapsed();
	return 0;
}
