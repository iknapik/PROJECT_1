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

void clean_db()
{
	std::remove("students_db.txt");
	std::remove("id_students_db.txt");
	std::remove("marks_db.txt");
	std::remove("id_marks_db.txt");
	std::remove("professors_db.txt");
	std::remove("id_professors_db.txt");
	std::remove("classes_db.txt");
	std::remove("id_classes_db.txt");
}

int main()
{
	//dla testów usuwa wszystkie pliki bazy danych
	clean_db();

	//dla testów
	Timer timer{};

	// sworzenie instancji bazy danych potrzebne do manipulacji bazą danych
	DatabaseModel db{};

	// tworzymy studenta {Imie, Nazwisko, PESEL, Miejscowośc, Ulica, opcjonalnie ID klasy}
	StudentInfo stud1{"Michal", "Kowalski", "9123819", "Wadowice", "Wiejska 3", 1};
	//tworzymy klase{Nazwa, który semestr}
	ClassInfo class1{"16i", 3};
	//tworzymy profesore{Imie, Nazwisko, Tytuł, {przedm1, przedm2, ...}}
	ProfessorInfo prof1{"Tomasz", "Wozniak", "Mgr", {"MD", "PE"}};
	//tworzymy ocene{id studenta, enum MARK - czyli ocena, data, enum SUBJECT - czyli przedmiot, id nauczyciela}
	MarkInfo mark1{4, MARK::_3, "04-01-2019", SUBJECT::MD, 3};
	
	//dodajemy do bazy danych za pomocą przeładowanych funkcji
	db.add(mark1);
	db.add(stud1);
	db.add(class1);
	db.add(prof1);

	//dodajemy dużo studentów dla sprawdzenia wydajności bazy danych
	for (int i = 1; i < 1000; ++i)
	{
		db.add(stud1);
		stud1.m_firstname = "Michal" + std::to_string(i);
		stud1.m_class_id = i % 3;
	}

	std::string str = "16i";
	
	// wyciągamy studentów z danej klasy
	for (auto &stud : db.get_students_by_class(str))
	{
		std::cout << stud << "\n";
	}
	timer.elapsed();
	return 0;
}
