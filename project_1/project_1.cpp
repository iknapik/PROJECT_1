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
	DatabaseModel db{};
	StudentInfo stud1{"Michal", "Kowalski", "9123819", "Wadowice", "Wiejska 3", 5};
	ClassInfo class1{"16i", 3};
	db.add(stud1);
	db.add(class1);
	std::cout << stud1 << " " << db.get_by_id<ClassInfo>(stud1.get_class_id());


	return 0;
}