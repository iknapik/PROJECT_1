// project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "StudentInfo.h"
#include "StudentDao.h"
#include "StudentModel.h"
#include "IdManager.h"
#include <cassert>

using namespace school;
bool operator<(const std::unique_ptr<StudentInfo>& s1, const std::unique_ptr<StudentInfo>& s2)
{
	std::cout << "hello";
	return s1->get_id() < s2->get_id();
}



int main()
{
	auto id = IdManager("student_id_db.txt");
	for (int i = 0; i < 100; i++)
	{
		std::cout << id.get_id() << "\n";

	}
	for (int j = 10 ; j < 50; j += 10)
	{
		id.release_id(j);
	}
	for (int i = 0; i < 5; i++)
	{
		std::cout << id.get_id() << "\n";

	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
