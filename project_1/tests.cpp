// project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// comment out if you don't want to run tests
#define TESTS
// -------------------------------------------
#ifdef TESTS
#include "Generator.h"
#endif
#include <iostream>
#include "DatabaseModel.h"
#include <cassert>
#include <array>
#include <cstdio>
#include <functional>
#include <chrono>
#include <iomanip>
#include <random>
template <class T>
	void assert_throw(T func, project::ErrorCode ec)
	{
		bool passed = false;
		unsigned id = 0;
		std::string mes;
		try
		{

			func();
		}
		catch (const project::DatabaseError& err)
		{
			if (err.error_code() == ec) passed = true;
			mes = err.what();
			id = err.id();
		}
		if (!passed) std::cout << "Error number: " << static_cast<int>(ec) << " error message: " << mes << " id: " << id;
		assert(passed);

	}


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
		auto secs = ticks / 1000;
		std::cout << "Took: " << secs << "." << std::setfill('0') << std::setw(3) << ticks - secs * 1000 << " seconds.\n";
		return ticks;
	}
	void reset()
	{
		elapsed();
		start = std::chrono::system_clock::now();
	}
};


using namespace project;
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
#ifndef TESTS



	//dla testów
	Timer timer{};

	// sworzenie instancji bazy danych potrzebne do manipulacji bazą danych
	{
		DatabaseModel db{};

		// tworzymy studenta {Imie, Nazwisko, PESEL, Miejscowośc, Ulica, opcjonalnie ID klasy}
		StudentInfo stud1{ "Michal", "Kowalski", "9123819", "Wadowice", "Wiejska 3", 1 };
		//tworzymy klase{Nazwa, który semestr}
		ClassInfo class1{ "16i", 3 };
		ClassInfo class2{ "14j", 1 };
		ClassInfo class3{ "10w", 5 };
		//tworzymy profesora {Imie, Nazwisko, Tytuł, {przedm1, przedm2, ...}}
		//przedmioty to zwykłe stringi co sie wpisze będzie ok
		ProfessorInfo prof1{ "Tomasz", "Szybki", "Mgr", {"MD", "PE"} , "93193919221"};
		ProfessorInfo prof2{ "Kamil", "Wozniak", "Dr", {"MD", "PE"} , "93193912221"};
		ProfessorInfo prof3{ "Wojciech", "Kowalski", "Inz", {"MD", "PE"} , "93299221"};
		ProfessorInfo prof4{ "Adam", "Miły", "Inż", {"MD", "PE"} , "93193919222"};
		ProfessorInfo prof5{ "Marek", "Marecki", "Mgr", {"MD", "PE"} , "93133619221"};
		//tworzymy ocene{id studenta, enum MARK - czyli ocena, data, enum SUBJECT - czyli przedmiot, id nauczyciela}
		//data to zwykły string
		MarkInfo mark1{ 1, MARK::_4, "04-01-2019", SUBJECT::MD, 3 };
		MarkInfo mark2{ 2, MARK::_5, "05-01-2019", SUBJECT::PE, 2 };
		MarkInfo mark3{ 3, MARK::_3_5, "06-01-2019", SUBJECT::PP, 1 };
		MarkInfo mark4{ 4, MARK::_4_5, "07-01-2019", SUBJECT::MD, 2 };
		MarkInfo mark5{ 4, MARK::_2_5, "08-01-2019", SUBJECT::PP, 3 };
		//dodajemy do bazy danych za pomocą przeładowanych funkcji

		//PRZY DODAWANIU NIE JEST POTRZEBNE USTALENIE ID
		//baza danych automatycznie przydziela ID, funkcja add modyfikuje obiekt nadająć mu odpowiednie ID
		//czyli po dodaniu db.add(class1); można użyć class1.get_id() aby uzyskać ID 

		try
		{
			db.add(class1);
			db.add(class2);
			db.add(class3);
		}
		catch (const DatabaseError& err)
		{
			std::cout << "Bład dodawania klasy, wiadomość błedu: " << err.what() << " " << err.id() << "\n";
			//jeżeli kod błedu dotyczy id err.id() wyświetla id które spowodawało bład
			//jeżeli kod błedu nie dotyczy id err.id() zwraca 0
		}
		try
		{
			db.add(prof1);
			db.add(prof2);
			db.add(prof3);
			db.add(prof4);
			db.add(prof5);
		}
		catch (const DatabaseError& err)
		{
			std::cout << "Błąd dodawania profesora, wiadomość błedu: " << err.what() << " " << err.id() << "\n";
		}

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
			catch (const DatabaseError& err)
			{
				std::cout << "Bład dodwania studenta, wiadomość błedu: " << err.what() << " " << err.id() << "\n";
			}
			stud1.m_firstname = "Michał" + std::to_string(i);
			stud1.m_class_id = i % 4 + 1;
		}
		//przy dodawaniu marka może wywalić exception gdy id studenta lub profesora nie istnieją
		try
		{
			db.add(mark1);
			db.add(mark2);
			db.add(mark3);
			db.add(mark4);
			db.add(mark5);
		}
		catch (const DatabaseError& err)
		{
			//WERSJA OGOLNA:
			//std::cout << "Bład dodawania oceny, wiadomość błedu: " << err.what() << " " << err.id();
			
			//WERSJA SZEGÓŁOWA:
			switch (err.error_code())
			{
			case ErrorCode::INVALID_STUDENT_ID:
				std::cout << "Bład dodawania oceny, podano niewłaściwe id studenta! id: " << err.id();
				break;
			case ErrorCode::INVALID_PROFESSOR_ID:
				std::cout << "Bład dodawania oceny, podano niewłaściwe id profesora! id: " << err.id();
				break;
			default:
				std::cout << "Bład dodawania oceny: " << err.what() << " " << err.id();
			}
		}
	}
	// ponowne ładowanie bazy danych jako symulacja ponownego otworzenia aplikacji
	{
		DatabaseModel db{};





		std::string str{ "10w" };
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
	}
	timer.elapsed();
#endif // !TESTS
	//PROSTE TESTY
#ifdef TESTS
	
	{
		std::cout << "Generating data..." << std::endl;
		Timer timer{false};
		Timer overall_timer{false};
		const unsigned students = 200;
		const unsigned classes = 10;
		const unsigned professors = 20;
		const unsigned marks = 1000;
		typedef void (DatabaseModel::*ups)(const StudentInfo&);
		typedef void (DatabaseModel::*upc)(const ClassInfo&);
		typedef void (DatabaseModel::*upp)(const ProfessorInfo&);
		typedef void (DatabaseModel::*upm)(const MarkInfo&);
		ups update_s = &DatabaseModel::update;
		upc update_c = &DatabaseModel::update;
		upp update_p = &DatabaseModel::update;
		upm update_m = &DatabaseModel::update;

		Generator gen{students, classes, professors, marks};
		{
			DatabaseModel db{};
			timer.reset();
			std::cout << "Populating database with " << std::endl;
			std::cout << gen.m_students.size() << " students, " << gen.m_classes.size() << " classes, " << gen.m_professors.size() << " professors and " << gen.m_marks.size() << " marks." << std::endl;
			gen.populate(db);
		}
		{
			timer.reset();
			std::cout << "Save, write test... " << std::flush;
			DatabaseModel db{};
			StudentInfo sinfo("Michael", "Jordan", "90909090901", "Gomoria,", "asdasd 213");
			typedef void (DatabaseModel::*ups)(const StudentInfo&);
			typedef void (DatabaseModel::*upc)(const ClassInfo&);
			typedef void (DatabaseModel::*upp)(const ProfessorInfo&);
			typedef void (DatabaseModel::*upm)(const MarkInfo&);
			ups update_s = &DatabaseModel::update;
			upc update_c = &DatabaseModel::update;
			upp update_p = &DatabaseModel::update;
			upm update_m = &DatabaseModel::update;

			for (auto &i : gen.m_students)
			{
				assert(i == db.get_by_id<StudentInfo>(i.get_id()));
			}
			std::cout << "." << std::flush;
			for (auto&i : gen.m_classes)
			{
				assert(i == db.get_by_id<ClassInfo>(i.get_id()));
			}
			std::cout << "." << std::flush;
			for (auto&i : gen.m_marks)
			{
				assert(i == db.get_by_id<MarkInfo>(i.get_id()));
			}
			std::cout << "." << std::flush;
			for (auto&i : gen.m_professors)
			{
				assert(i == db.get_by_id<ProfessorInfo>(i.get_id()));
			}
			assert(db.check_integrity());
			std::cout << "Done!" << std::endl;
			//*************************************************************//

			timer.reset();
			std::cout << "Update test... " << std::flush;
			for (size_t i = 1; i < students; i += students / 5)
			{
				gen.m_students[i].m_firstname = "Bolek";
				db.update(gen.m_students[i]);
			}
			for (size_t i = 1; i < classes; i += classes / 5)
			{
				gen.m_classes[i].m_semester = 8;
				db.update(gen.m_classes[i]);
			}
			for (size_t i = 1; i < professors; i += professors / 5)
			{
				gen.m_professors[i].m_first_name = "Mietek";
				db.update(gen.m_professors[i]);
			}
			for (size_t i = 1; i < marks; i += marks / 5)
			{
				gen.m_marks[i].m_value = MARK::_2;
				db.update(gen.m_marks[i]);
			}
			std::cout << "." << std::flush;
		}
		{
			DatabaseModel db{};	
			for (size_t i = 1; i < students; i += students / 5)
			{
				auto stud = db.get_by_id<StudentInfo>(gen.m_students[i].get_id());
				assert(stud.m_firstname == "Bolek");
				assert(db.remove_student(stud));
			}
			std::cout << "." << std::flush;
			for (size_t i = 1; i < classes; i += classes / 5)
			{
				auto cls = db.get_by_id<ClassInfo>(gen.m_classes[i].get_id());
				assert(cls.m_semester == 8);
				if (db.get_students_by_class(cls.get_id()).empty())
					assert(db.remove_class(cls));
				else
					assert_throw(std::bind(&DatabaseModel::remove_class, &db, cls), ErrorCode::CLASS_IN_USE);
			}
			std::cout << "." << std::flush;
			for (size_t i = 1; i < professors; i += professors / 5)
			{
				auto prof = db.get_by_id<ProfessorInfo>(gen.m_professors[i].get_id());
				assert(prof.m_first_name == "Mietek");
				assert(db.remove_professor(prof));
			}
			assert(db.check_integrity());
			std::cout << "Done!" << std::endl;
		}
		//*************************************************************//
		{
			timer.reset();
			std::cout << "Remove test... ";
			DatabaseModel db{};
			std::cout << "." << std::flush;
			for (size_t i = 1; i < students; i += students / 5)
			{
				unsigned id = gen.m_students[i].get_id();
				assert_throw(std::bind(&DatabaseModel::get_by_id<StudentInfo>, &db, id), ErrorCode::INVALID_ID);
				assert(db.get_marks_by_student_id(id).empty());
			}	
			std::cout << "." << std::flush;
			for (size_t i = 1; i < professors; i += professors / 5)
			{
				unsigned id = gen.m_professors[i].get_id();
				assert_throw(std::bind(&DatabaseModel::get_by_id<ProfessorInfo>, &db, id), ErrorCode::INVALID_ID);
			}
			std::cout << "." << std::flush;
			auto cls = *db.get_all<ClassInfo>().begin();
			{
				std::string name = cls.second->m_name;
				for (auto& stud : db.get_students_by_class(name))
				{
					assert(db.remove_student(stud));
				}
				assert(db.remove_class_by_id(cls.first));
			}
			std::cout << "Done!" << std::endl;	

			
			timer.reset();
			std::cout << "find_students tests... ";
			{
				auto student = *db.get_all<StudentInfo>().begin()->second;
				assert(db.find_students("$$$$$$$$$$$$$$$").empty());
				assert(!db.find_students(student.m_address).empty());
				assert(!db.find_students(student.m_firstname).empty());
				assert(!db.find_students(student.m_lastname).empty());
				assert(!db.find_students(student.m_PESEL).empty());
				assert(!db.find_students(student.m_city).empty());
			}
			{
				assert(db.check_integrity());
				auto student = *(--db.get_all<StudentInfo>().end())->second;
				assert(db.find_students("           ").empty());
				assert(!db.find_students(student.m_address).empty());
				assert(!db.find_students(student.m_firstname).empty());
				assert(!db.find_students(student.m_lastname).empty());
				assert(!db.find_students(student.m_PESEL).empty());
				assert(!db.find_students(student.m_city).empty());
			}
			std::cout << "Done!" << std::endl;
			timer.reset();
			std::cout << "get_students_by_class tests...";
			{
				std::map<unsigned, std::set<unsigned>> cls_stud;
				for (auto& i : db.get_all<StudentInfo>())
				{
					cls_stud[i.second->m_class_id].insert(i.first);
				}
				for (auto& pair : cls_stud)
				{
					
					auto studs = db.get_students_by_class(pair.first);
					for (auto &onestud : studs)
					{
						assert(pair.second.find(onestud.get_id()) != pair.second.end());
					}
				}

			}
			std::cout << "Done!" << std::endl;
			timer.elapsed();
			timer.reset();
			std::cout << "add/update/remove exception tests...";
			{
				ClassInfo cinfo = *(db.get_all<ClassInfo>().begin()->second);
				ProfessorInfo pinfo = *(db.get_all<ProfessorInfo>().begin()->second);
				StudentInfo sinfo = *(db.get_all<StudentInfo>().begin()->second);
				MarkInfo minfo = *(db.get_all<MarkInfo>().begin()->second);
				cinfo.m_name = "14i,";
				pinfo.m_subjects[0] = "MD,";
				sinfo.m_address = "Zielna,29";
				minfo.m_date = "12,03,2019";
				assert_throw(std::bind(&DatabaseModel::add<ClassInfo>, &db, cinfo), ErrorCode::DISSALOWED_CHARACTER);
				assert_throw(std::bind(&DatabaseModel::add<ProfessorInfo>, &db, pinfo), ErrorCode::DISSALOWED_CHARACTER);
				assert_throw(std::bind(&DatabaseModel::add<StudentInfo>, &db, sinfo), ErrorCode::DISSALOWED_CHARACTER);
				assert_throw(std::bind(&DatabaseModel::add<MarkInfo>, &db, minfo), ErrorCode::DISSALOWED_CHARACTER);
				assert_throw(std::bind(update_c, &db, cinfo), ErrorCode::DISSALOWED_CHARACTER);
				assert_throw(std::bind(update_p, &db, pinfo), ErrorCode::DISSALOWED_CHARACTER);
				assert_throw(std::bind(update_s, &db, sinfo), ErrorCode::DISSALOWED_CHARACTER);
				assert_throw(std::bind(update_m, &db, minfo), ErrorCode::DISSALOWED_CHARACTER);
				StudentInfo sinfo_id("H", "H", "92313131113", "H", "H", cinfo.get_id());
				ProfessorInfo pinfo_id("H", "H", "H", { "ff" }, "29192919999");
				ClassInfo cinfo_id("ale", 3);
				MarkInfo minfo_id(sinfo.get_id(), MARK::_2_5, "fafaf", SUBJECT::MD, pinfo.get_id());
				assert_throw(std::bind(update_c, &db, cinfo_id), ErrorCode::INVALID_CLASS_ID);
				assert_throw(std::bind(update_p, &db, pinfo_id), ErrorCode::INVALID_PROFESSOR_ID);
				assert_throw(std::bind(update_s, &db, sinfo_id), ErrorCode::INVALID_STUDENT_ID);
				assert_throw(std::bind(update_m, &db, minfo_id), ErrorCode::INVALID_MARK_ID);
				sinfo_id.m_class_id = 99999999;
				assert_throw(std::bind(&DatabaseModel::add<StudentInfo>, &db, sinfo_id), ErrorCode::INVALID_CLASS_ID);
				minfo_id.m_student_id = 99999999;
				assert_throw(std::bind(&DatabaseModel::add<MarkInfo>, &db, minfo_id), ErrorCode::INVALID_STUDENT_ID);
				minfo_id.m_student_id = sinfo.get_id();
				minfo_id.m_professor_id = 99999999;
				assert_throw(std::bind(&DatabaseModel::add<MarkInfo>, &db, minfo_id), ErrorCode::INVALID_PROFESSOR_ID);

				assert_throw(std::bind(&DatabaseModel::get_by_id<StudentInfo>, &db, 9999999), ErrorCode::INVALID_ID);
				assert_throw(std::bind(&DatabaseModel::get_by_id<ClassInfo>, &db, 9999999), ErrorCode::INVALID_ID);
				assert_throw(std::bind(&DatabaseModel::get_by_id<MarkInfo>, &db, 9999999), ErrorCode::INVALID_ID);
				assert_throw(std::bind(&DatabaseModel::get_by_id<ProfessorInfo>, &db, 9999999), ErrorCode::INVALID_ID);

				pinfo = *(db.get_all<ProfessorInfo>().begin()->second);
				sinfo = *(db.get_all<StudentInfo>().begin()->second);
				pinfo.m_PESEL = "09312";
				sinfo.m_PESEL = "fjafa";

				assert_throw(std::bind(&DatabaseModel::add<ProfessorInfo>, &db, pinfo), ErrorCode::INVALID_PESEL);
				assert_throw(std::bind(&DatabaseModel::add<StudentInfo>, &db, sinfo), ErrorCode::INVALID_PESEL);
				assert_throw(std::bind(update_p, &db, pinfo), ErrorCode::INVALID_PESEL);
				assert_throw(std::bind(update_s, &db, sinfo), ErrorCode::INVALID_PESEL);
				assert(db.check_integrity());
				std::cout << " Done!\n";
				timer.elapsed();

			}

		}
		
		std::cout << "All tests ";
		overall_timer.elapsed();
	}
#endif
	return 0;
}
