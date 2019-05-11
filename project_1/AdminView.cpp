#include "AdminView.h"
#include <iostream>
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
		std::cout << "1 - Change Admin password\n2 - Change Students password\n3 -  Change Professors password\n4 - Back\n";
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
}

void AdminView::manage_professors()
{
}
void AdminView::manage_marks()
{
}
void AdminView::manage_classes()
{
}
void AdminView::statistics()
{
}
