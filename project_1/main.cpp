#include "DatabaseModel.h"
#include <iostream>
#include <iomanip>
#include <string_view>
#include "AdminView.h"
#include "Generator.h"
#include "StudentView.h"


bool stop = false;
using namespace project;
//enum used to tell what privileges should use have
enum class USERTYPE
{
	ADMIN, PROFESSOR, STUDENT
};


//helper for geting userinput
void get_response(std::string& buffer);
//<<<<<<< HEAD
std::string zmiana(std::string& ciag);

unsigned get_prof_id_from_pesel(const DatabaseModel& db, const std::string& str);
unsigned get_id_from_pesel(const DatabaseModel& db, const std::string& str);
//bool pass(std::string ciag, std::string passw);
//=======

bool is_exit_requested(std::string str);

void clean_db()
{
	std::remove("std_db.txt");
	std::remove("id_std_db.txt");
	std::remove("mrk_db.txt");
	std::remove("id_mrk_db.txt");
	std::remove("prs_db.txt");
	std::remove("id_prs_db.txt");
	std::remove("cls_db.txt");
	std::remove("id_cls_db.txt");
	std::remove("passwords.txt");
}
//>>>>>>> 82c63ab01d037b4beb851d45cac974563b7bcb43

//view where user can login returns info about privileges
USERTYPE login_view(const DatabaseModel& db);
unsigned get_user_id(USERTYPE type, const DatabaseModel& db);
bool is_valid_PESEL(const std::string_view& strv);

int main()
{
	clean_db();
	DatabaseModel db("cls_db.txt", "std_db.txt", "prs_db.txt", "mrk_db.txt");
	//generator if you want for testing
	const unsigned students = 200;
	const unsigned classes = 10;
	const unsigned professors = 20;
	const unsigned marks = 1000;
	Generator gen(students, classes, professors, marks);
	gen.populate(db);

login:
	stop = false;
	USERTYPE type = login_view(db);
	if (stop) return 1;
	unsigned user_id = get_user_id(type, db);
	if (stop) return 1;
	// jeżeli id jest = 0 to coś jest nie tak, chyba że to jest admin
	if (type != USERTYPE::ADMIN && !user_id)
	{
		goto login;
	}

	switch (type)
	{
	case USERTYPE::ADMIN:
	{AdminView admin_view(db);
	admin_view.menu();
	if (admin_view.is_exit_requested()) return 2;
	else goto login;
	break; }
	case USERTYPE::STUDENT:
	{StudentView student_view(db, user_id);
	student_view.menu();
	if (student_view.get_stop()) return 3;

	else goto login;
	break; }
	case USERTYPE::PROFESSOR:
		std::cout << "Nothing Here folks!\n";
		break;
	default:
		break;
	}
	/*
	case USERTYPE::PROFESSOR:
		ProfessorView professor_view(db);
		professor_view.menu();
	if (aprofessor_view.is_exit_requested()) return 2;
	else goto login;
	break;
	}*/


	return 0;
}

USERTYPE login_view(const DatabaseModel& db)
{
	std::string response;
	std::cout << "--- Welcome to the online register ---" << std::endl;
	std::cout << std::endl;
	std::cout << "If you want to leave, press q|exit|quit\n";
	std::cout << "Type your Username: Admin|Professor|Student" << std::endl;

	while (true)
	{
		get_response(response);
		zmiana(response);

		if (is_exit_requested(response)) { stop = true; return USERTYPE::STUDENT; }
		if (response == "ADMIN") { return  USERTYPE::ADMIN; }
		else if (response == "PROFESSOR") { return USERTYPE::PROFESSOR; }
		else if (response == "STUDENT") { return  USERTYPE::STUDENT; }
		std::cout << "Try again\n";
	}
}
unsigned get_user_id(USERTYPE type, const DatabaseModel& db)
{
	std::string response, password;
	unsigned id;
	int i = 1;
	switch (type)
	{
	case USERTYPE::STUDENT:
		while (true)
		{
			std::cout << "PESEL:\n";

			get_response(response);
			if (is_exit_requested(response)) { stop = true; return 0; }

			if (!is_valid_PESEL(response))
			{
				std::cout << "Invalid PESEL format! try  again!\n";
				continue;
			}
			id = get_id_from_pesel(db, response);
			if (!id)
			{
				std::cout << "User with that PESEL not found! try again!\n";
				continue;
			}
			password = db.get_by_id<StudentInfo>(id).get_password();

			password_:
			std::cout << "Password:\n";

			get_response(response);
			if (is_exit_requested(response)) { stop = true; return 0; }

			if (response == password) { std::cout << "access granted!\n"; return id; }
			else {
				std::cout << "Wrong password!";
				if (i < 3)
				{
					std::cout << " This is your " << i << " attempt, only " << 3 - i << " left!" << std::endl;
				}
				if (i == 3)
				{
					std::cout << " Your account has been blocked, please contact with the administrator" << std::endl;
					stop = true; return 0;
				}
				i++;
				goto password_;
			}
			//chcialam zrobic kolorki, ze taki czerwony error, ale u mnie nie dziala: "\33[0;31m" << "Wrong password!" << "\33[0m"
		}
		break;
	case USERTYPE::ADMIN:
		password = db.get_admin_password();
		while (true)
		{
			std::cout << "Password:\n";

			get_response(response);
			if (is_exit_requested(response)) { stop = true; return 0; }

			if (response == password) { std::cout << "access granted!\n"; return 0; }
			else {
				std::cout << "Wrong password!";
				if (i < 3)
				{
					std::cout << " This is your " << i << " attempt, only " << 3 - i << " left!" << std::endl;
				}
				if (i == 3)
				{
					std::cout << " Your account has been blocked, please contact with the administrator" << std::endl;
					stop = true; return 0;
				}
				i++;
			}
		}
		break;
	case USERTYPE::PROFESSOR:
		while (true)
		{
			std::cout << "PESEL:\n";

			get_response(response);
			if (is_exit_requested(response)) { stop = true; return 0; }

			if (!is_valid_PESEL(response))
			{
				std::cout << "Invalid PESEL format! try  again!\n";
				continue;
			}
			id = get_prof_id_from_pesel(db, response);
			if (!id)
			{
				std::cout << "User with that PESEL not found! try again!\n";
				continue;
			}
			password = db.get_by_id<ProfessorInfo>(id).get_password();

			prof_password_:
			std::cout << "Password:\n";

			get_response(response);
			if (is_exit_requested(response)) { stop = true; return 0; }

			if (response == password) { std::cout << "access granted!\n"; return id; }
			else {
				std::cout << "Wrong password!";
				if (i < 3)
				{
					std::cout << " This is your " << i << " attempt, only " << 3 - i << " left!" << std::endl;
				}
				if (i == 3)
				{
					std::cout << " Your account has been blocked, please contact with the administrator" << std::endl;
					stop = true; return 0;
				}
				i++;
				goto prof_password_;
			}
			//chcialam zrobic kolorki, ze taki czerwony error, ale u mnie nie dziala: "\33[0;31m" << "Wrong password!" << "\33[0m"
		}
		break;

	default:
		break;
	}
}

bool is_valid_PESEL(const std::string_view& strv)
{
	if (strv.size() == 11)
	{
		for (auto& chr : strv)
		{
			if (!isdigit(chr)) return false;
		}
		return true;
	}
	return false;
}
void get_response(std::string& buffer)
{
	while (true) 
	{
		std::cin >> buffer;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			//continue;
		}
		break;
	}
    
}


//sprawia ze username i exit nie jest czuly na wielkosc liter
std::string zmiana(std::string& ciag)
{ int i=0;
    
    while(ciag[i]!=0)
    {
        if (ciag[i]>96 && ciag[i]<122)
        {
            ciag[i]=(ciag[i]-32);
        }
        i++;
    }
    return ciag;
}

unsigned get_prof_id_from_pesel(const DatabaseModel& db, const std::string& str)
{
    auto prof_map = db.get_all<ProfessorInfo>();
	for (auto& pair : prof_map)
	{
		if (pair.second->m_PESEL == str) return pair.first;
	}
    return 0;
}


unsigned get_id_from_pesel(const DatabaseModel& db, const std::string& str)
{
    auto stud_list = db.find_students(str);
    if (!stud_list.empty())
    {
        if (stud_list.size() == 1)
            return stud_list.begin()->get_id();
        //nie wiem w sumie czy elsy sa nam tu potrzebne bo juz haslo samo w sobie sprawdza poprawnosc pesela, jakby byl on bledny lub nie bylo wynikow to bysmy sie nie zalogowali
        else
        {
            // za dużo wyników, przykładowo jak ktos wpisze 932 tylko
            return 0;
        }
    }
    else //brak wyników
        return 0;
}
bool is_exit_requested(std::string str)
{
	if (str == "q" || str == "exit" || str == "quit" || str == "Q" || str == "EXIT" || str == "QUIT") { stop = true; return true; }
	return false;
}

/*
bool pass(std::string ciag, std::string passw)
{
    if(sizeof(ciag)==11 && ciag == passw)
        return true;
    else if(sizeof(ciag)<11)
    {std::cout<<"Password is too short, please try again"<<std::endl;
        return false;}
    else if(sizeof(ciag)>11)
    {std::cout<<"Password is too long, please try again"<<std::endl;
        return false;}
    
    
}
 */