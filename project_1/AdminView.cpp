#include "AdminView.h"
#include "StudentInfo.h"
#include "DatabaseModel.h"
#include <iostream>
using namespace project;


	typedef unsigned uint;

	void AdminView::menu()
	{
		while (true)
		{
			std::cout << "1 - Manage Passwords\n2 - Manage Students\n3 - Manage Professors\n4 - Manage Marks\n5 - Manage Classes 6 - Statistics 7 - Back to Login\n";
			get_response();
			if (m_stop) return;
			//you cannot switch case with strings so first we check it string is length 1 then we switch case on first character
			if (m_response.size() == 1)
			{
				switch (m_response[0])
				{
				case '1':
					manage_passwords();
					break;
				case '2':
					manage_students();
					break;
				case '3':
					manage_professors();
					break;
				case '4':
					manage_marks();
					break;
				case '5':
					manage_classes();
					break;
				case '6':
					statistics();
					break;
				case '7':
					return;
				default:
					std::cout << "Wrong Input!\n";
				}
			}
			if (m_stop) return;
		}

	}

	void AdminView::manage_passwords()
	{
		std::string password_helper;
		while (true)
		{
			//can be made to use strings with help of an enum
			std::cout << "1 - Change Admin password\n2 - Change Students password\n3 - Change Professors password\n4 - Back\n";
			get_response();
			if (m_stop) return;
			if (m_response.size() == 1)
			{
				switch (m_response[0])
				{
				case '1':
					std::cout << "type new admin password:\n";
					get_response();
					password_helper = m_response;
					std::cout << "retype new admin password:\n";
					get_response();
					if (m_response == password_helper)
					{
						if (m_db.set_admin_password(m_response)) std::cout << "Password changed succesfully!\n";
						else std::cout << "Password change failed, did you use ',' in it?";
					}
					else { std::cout << "passwords don't match!\n"; }
					break;
				case '2':
					std::cout << "type new student password:\n";
					get_response();
					password_helper = m_response;
					std::cout << "retype new student password:\n";
					get_response();
					if (m_response == password_helper)
					{
						if (m_db.set_student_password(m_response)) std::cout << "Password changed succesfully!\n";
						else std::cout << "Password change failed, did you use ',' in it?";
					}
					else { std::cout << "passwords don't match!\n"; }
					break;
				case '3':
					std::cout << "type new professor password:\n";
					get_response();
					password_helper = m_response;
					std::cout << "retype new professor password:\n";
					get_response();
					if (m_response == password_helper)
					{
						if (m_db.set_professor_password(m_response)) std::cout << "Password changed succesfully!\n";
						else std::cout << "Password change failed, did you use ',' in it?";
					}
					else { std::cout << "passwords don't match!\n"; }
					break;
				case '4':
					return;
				default:
					std::cout << "Wrong Input!\n";
				}

			}
		}


		get_response();
	}
	void AdminView::manage_students()
	{
		std::cout << "1 - See all students \n";
		std::cout << "2 - Modify student \n";
		std::cout << "3 - Add student \n";
		std::cout << "4 - Back \n";
		get_response();
		if (m_stop) return;
		if (m_response.size() == 1)
		{
			switch (m_response[0])
			{
				case '1':
				{
					std::list<StudentInfo> allstudents;
					allstudents = m_db.get_students();
					for (auto& it :allstudents)
					{
						std::cout << "\n";
						std::cout << it.get_id() << " Name: " << it.m_firstname << " " << it.m_lastname << "\n";
						std::cout << "Class number:" << it.m_class_id << " PESEL:" << it.m_PESEL << "\n";
						std::cout << it.m_address << " " << it.m_city << "\n";
						std::cout << "\n";
					}
				}
				break;
				case '2':
				{
					std::cout << "Input Student ID \n";
					uint id;
					std::cin >> id;
					std::list<StudentInfo> student;
					student = m_db.get_student_by_id(id);
					if (student.empty()) {
						std::cout << "Invalid Student ID \n";
						break;
					}

					StudentInfo it = student.front();
					bool menu1 = true;
					while (menu1) {
						std::cout << it.get_id() << " Name: " << it.m_firstname << " " << it.m_lastname << "\n";
						std::cout << "Class number:" << it.m_class_id << " PESEL:" << it.m_PESEL << "\n";
						std::cout << it.m_address << " " << it.m_city << "\n";

						std::cout << "What do you want to modify: \n";
						std::cout << "1 - Name \n";
						std::cout << "2 - Surname \n";
						std::cout << "3 - Class number \n";
						std::cout << "4 - PESEL \n";
						std::cout << "5 - adress \n";
						std::cout << "6 - city \n";
						std::cout << "7 - commit chagnes to DB \n";
						std::cout << "8 - remove this student \n";
						std::cout << "9 - exit ( use 7 to save if you don't changes won't be written to DB) \n";


						std::string whatmodify = get_response2();
						switch (whatmodify[0]) {
						case '1':
						{
							std::cout << "Current NAME:" << it.m_firstname << "Insert new name \n";
							std::string newname = "";
							std::cin >> newname;
							if (check_name(newname)) {
								std::cout << "Name accepted. Changed name from " << it.m_firstname << "to " << newname << "\n";
								it.m_firstname = newname;
							}
							else {
								std::cout << "New name rejected. Only English letters from a-z A-Z are accepted \n";
							}

						}
							break;
						case '2':
						{

						}
							//copy name funtion here since surnames are the same, might allow for - in surnames check as some ppl have 2part surname

							break;
						case '3':
						{

						}
							break;
						case '4':
						{
							std::cout << "Current PESEL:" << it.m_PESEL << "Insert new PESEL \n";
							std::string newPESEL = "";
							std::cin >> newPESEL;
							if (check_PESEL(newPESEL)) {
								std::cout << "PeSEL accepted. Changed PeSEL from " << it.m_PESEL << " to " << newPESEL << "\n";
								it.m_PESEL = newPESEL;
							}
							else {
								std::cout << "New PESEL rejected. PESEL must be 11 digits (0-9) \n";
							}
						}
							break;
						case '5':
						{

						}
							break;
						case '6':
						{

						}
							break;
						case '7':
						{
							std::cout << "Are you SURE you want to update the Student in the Database? Type Y to confirm  \n";
							std::cout << "the changes cannot be reverted \n";
							char s = 'o';
							std::cin >> s;
							if (s == 'Y') {
								m_db.update(it);
								std::cout << "Updated. Have a great day";
							}
							else {
								std::cout << "going back \n";

							}

							

						}
							break;
						case '8':
						{
							std::cout << "Are you SURE you want to remove the Student\n";
							std::cout << it.get_id() << " Name: " << it.m_firstname << " " << it.m_lastname << "\n";
							std::cout << "Class number:" << it.m_class_id << " PESEL:" << it.m_PESEL << "\n";
							std::cout << it.m_address << " " << it.m_city << "\n";
								
							std::cout <<  "from the Database? Type Y to confirm  \n";

							std::cout << "the changes cannot be reverted \n";
							char s = 'o';
							std::cin >> s;
							if (s == 'Y') {
								m_db.remove_student_by_id(it.get_id());
								std::cout << "Student removed. Have a nice day";
								menu1 = false;
								break;
							}
							else {
								std::cout << "going back \n";

							}

							
						}
							break;
						case '9':
						{
							menu1 = false;
							break;
						}
						
						default:
						{
							std::cout << "Wrong Input!\n";
						}
							

						}
					}
					


				}
				break;

				case '3':
				{
					StudentInfo it;
					uint search = 1;

					std::list<StudentInfo> e;
					bool found = false;
					while (!found) {
						e = m_db.get_student_by_id(search);
						if (e.empty()) {
							it.set_id(search);
							found = true;
						}
						else {
							search++;
						}
					}
					
					

					bool menu1 = true;
					while (menu1) {
						std::cout << "ID: " << it.get_id() << " Name: " << it.m_firstname << " " << it.m_lastname << "\n";
						std::cout << "Class number:" << it.m_class_id << " PESEL:" << it.m_PESEL << "\n";
						std::cout << it.m_address << " " << it.m_city << "\n";

						std::cout << "Please enter new data for student: \n";
						std::cout << "1 - Name \n";
						std::cout << "2 - Surname \n";
						std::cout << "3 - Class number \n";
						std::cout << "4 - PESEL \n";
						std::cout << "5 - adress \n";
						std::cout << "6 - city \n";
						std::cout << "7 - commit chagnes to DB \n";
						std::cout << "9 - exit ( use 7 to save if you don't changes won't be written to DB) \n";


						std::string whatmodify = get_response2();
						switch (whatmodify[0]) {
						case '1':
						{
							std::cout << "Current NAME:" << it.m_firstname << "Insert new name \n";
							std::string newname = "";
							std::cin >> newname;
							if (check_name(newname)) {
								std::cout << "Name accepted. Changed name from " << it.m_firstname << "to " << newname << "\n";
								it.m_firstname = newname;
							}
							else {
								std::cout << "New name rejected. Only English letters from a-z A-Z are accepted \n";
							}

						}
						break;
						case '2':
						{

						}
						//copy name funtion here since surnames are the same, might allow for - in surnames check as some ppl have 2part surname

						break;
						case '3':
						{

						}
						break;
						case '4':
						{
							std::cout << "Current PESEL:" << it.m_PESEL << "Insert new PESEL \n";
							std::string newPESEL = "";
							std::cin >> newPESEL;
							if (check_PESEL(newPESEL)) {
								std::cout << "PeSEL accepted. Changed PeSEL from " << it.m_PESEL << " to " << newPESEL << "\n";
								it.m_PESEL = newPESEL;
							}
							else {
								std::cout << "New PESEL rejected. PESEL must be 11 digits (0-9) \n";
							}
						}
						break;
						case '5':
						{

						}
						break;
						case '6':
						{

						}
						break;
						case '7':
						{
							std::cout << "Are you SURE you want to update the Student in the Database? Type Y to confirm  \n";
							std::cout << "the changes cannot be reverted \n";
							char s = 'o';
							std::cin >> s;
							if (s == 'Y') {
								m_db.update(it);
								std::cout << "Updated. Have a great day";
							}
							else {
								std::cout << "going back \n";

							}



						}
						break;
						
						
						case '9':
						{
							menu1 = false;
							break;
						}

						default:
						{
							std::cout << "Wrong Input!\n";
						}


						}
					}



				}
				break;
				case '4':

				return;
				default:
				{
					std::cout << "Wrong Input!\n";
				}

			}
		}

	}

	void AdminView::manage_professors()
	{
		std::cout << "1 - See professor list\n2 - Modify professor\n3 - Remove professor\n4-Add professor\n5 - Back\n";
		get_response();
		if (m_stop) return;
		if (m_response.size() == 1)
		{
			switch (m_response[0])
			{
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			case '4':
				break;
			case '5':
				break;
			}
		}
	}
	void AdminView::manage_marks()
	{
		std::cout << "1 - See professor's given marks\n2 -See student's received marks\n3 - Back\n";
		get_response();
		if (m_stop) return;
		if (m_response.size() == 1)
		{
			switch (m_response[0])
			{
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			}
		}
	}
	void AdminView::manage_classes()
	{
		/*
		std::cout << "1 - See classes list\n2 \n3 - Remove professor\n4-Add professor\n5 - Back\n";
		get_response();
		if (m_stop) return;
		if (m_response.size() == 1)
		{
			switch (m_response[0])
			{

			}
		}
		*/

	}
	void AdminView::statistics()
	{

		std::cout << "1 - See all info\n 2 -See \n 3 - Back\n";
		get_response();
		if (m_stop) return;
		if (m_response.size() == 1)
		{
			switch (m_response[0])
			{
			case '1':
			{	//has to be in brackets because of goto statement
				std::map< const unsigned int, std::shared_ptr<const ProfessorInfo>> professors;

				std::map< const unsigned int, std::shared_ptr<const StudentInfo>> students;
				students = m_db.get_all<StudentInfo>();

				for (auto it = students.cbegin(); it != students.cend(); ++it)
				{
					std::cout << it->first << " Name: " << it->second->m_firstname << " " << it->second->m_lastname << "\n";
					std::cout << "Class number:" << it->second->m_class_id << " PESEL:" << it->second->m_PESEL << "\n";
					std::cout << it->second->m_address << " " << it->second->m_city << "\n";

				}

				professors = m_db.get_all<ProfessorInfo>();

				for (auto it = professors.cbegin(); it != professors.cend(); ++it)
				{
					std::cout << it->first << "" << it->second->m_title << " " << it->second->m_first_name << it->second->m_last_name << "\n";
					std::cout << "Subjects:" << std::endl;
					for (auto i = it->second->m_subjects.begin(); i != it->second->m_subjects.end(); i++) {
						std::cout << *i << std::endl;
					}

				}
			}

			break;
			case '2':
				break;
			case '3':
				return;
			default:
			{
				std::cout << "Wrong Input!\n";
			}
			}
		}

		




	}
