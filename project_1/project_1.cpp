// project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DatabaseModel.h"
#include <cassert>
#include <array>
#include <cstdio>
#include <chrono>
#include <iomanip>

// timer class made just for testing purposes
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
	ClassInfo class2{"14j", 1 };
	ClassInfo class3{"10w", 5 };
	//tworzymy profesora {Imie, Nazwisko, Tytuł, {przedm1, przedm2, ...}}
	ProfessorInfo prof1{"Tomasz", "Szybki", "Mgr", {"MD", "PE"}};
	ProfessorInfo prof2{ "Kamil", "Wozniak", "Dr", {"MD", "PE"} };
	ProfessorInfo prof3{ "Wojciech", "Kowalski", "Inz", {"MD", "PE"} };
	ProfessorInfo prof4{ "Adam", "Miły", "Inż", {"MD", "PE"} };
	ProfessorInfo prof5{ "Marek", "Marecki", "Mgr", {"MD", "PE"} };
	//tworzymy ocene{id studenta, enum MARK - czyli ocena, data, enum SUBJECT - czyli przedmiot, id nauczyciela}
	MarkInfo mark1{1, MARK::_4, "04-01-2019", SUBJECT::MD, 3};
	MarkInfo mark2{ 2, MARK::_5, "05-01-2019", SUBJECT::PE, 2 };
	MarkInfo mark3{ 3, MARK::_3_5, "06-01-2019", SUBJECT::PP, 1 };
	MarkInfo mark4{ 4, MARK::_4_5, "07-01-2019", SUBJECT::MD, 2 };
	MarkInfo mark5{ 4, MARK::_2_5, "08-01-2019", SUBJECT::PP, 3 };
	//dodajemy do bazy danych za pomocą przeładowanych funkcji

	db.add(class1);
	db.add(class2);
	db.add(class3);
	db.add(prof1);
	db.add(prof2);
	db.add(prof3);
	db.add(prof4);
	db.add(prof5);	

	// przy dodawaniu studenta może wywalić exception gdy klasa podana w StudentInfo nie istnieje
	// dodawanie studenta aktualizuje StudentInfo w poprawne ID
	// klasa o id 4 nie istnieje wiec wywala błąd:
	for (int i = 1; i < 10; ++i)
	{
		try
		{

			db.add(stud1);
			std::cout << "id dodanego studenta: " << stud1.get_id() << "\n";
		}
		catch (const InvalidField& err)
		{
			std::cout << err.what() << " " << err.id() << "\n";
		}
		stud1.m_firstname = "Michał" + std::to_string(i);
		stud1.m_class_id = i % 4 + 1;
	}
	//przy dodawaniu marka może wywalić exception gdy id studenta lub profesora nie istnieją
	db.add(mark1);
	db.add(mark2);
	db.add(mark3);
	db.add(mark4);
	db.add(mark5);	

	std::string str = "10w";
	std::cout << "\nStudenci w danej klasie:\n====================================\n";
	// wyciągamy studentów z danej klasy, możemy również po prostu podać class id
	for (auto &stud : db.get_students_by_class(str))
	{
		std::cout << stud << "\n";
	}
	// szukamy studentów:
	str = "MiChal WieJSKA 3";
	std::cout << "\nStudenci po szukanej frazie:\n====================================\n";
	for (auto &stud : db.find_students(str))
	{
		stud.get_id();
		std::cout << stud << "\n";
	}
	//szukamu marków po ID studenta
	std::cout << "\nMarki po ID studenta:\n====================================\n";
	for (auto& mark : db.get_marks_by_student_id(4))
	{
		std::cout << mark << "\n";
	}
	//szukamy marków po ID profesora
	std::cout << "\nMarki po ID profesora:\n====================================\n";
	for (auto& mark : db.get_marks_by_professor_id(3))
	{
		std::cout << mark << "\n";
	}
	//lista profesorów:
	std::cout << "\nProfesorowie:\n====================================\n";

	for (auto& prof : db.get_all<ProfessorInfo>())
	{
		std::cout << std::setw(7) << "id: " << prof.first << " " << *prof.second << "\n";
	}
	//lista class:
	std::cout << "\nKlasy:\n====================================\n";

	for (auto& cls : db.get_all<ClassInfo>())
	{
		std::cout << std::setw(7) << "id: " << cls.first << " " << *cls.second << "\n";
	}
	//lista studentów:
	std::cout << "\nStudenci:\n====================================\n";
	for (auto& cls : db.get_all<StudentInfo>())
	{
		std::cout << *cls.second << "\n";
	}
	//lista marków:
	std::cout << "\nMarki:\n====================================\n";
	for (auto& mark : db.get_all<MarkInfo>())
	{
		std::cout << *mark.second << "\n";
	}

	timer.elapsed();
	return 0;
}
