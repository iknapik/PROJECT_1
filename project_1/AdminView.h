#ifndef ADMIN_VIEW_H_
#define ADMIN_VIEW_H_
#include "DatabaseModel.h"
#include <iostream>

class AdminView
{
	project::DatabaseModel& m_db;
	bool m_stop = false;
	std::string m_response;
public:
	explicit AdminView(project::DatabaseModel& database) : m_db(database) {}
	void menu();
	bool is_exit_requested() { return m_stop; }
private:
	void manage_passwords();
	void manage_students();
	void manage_professors();
	void manage_marks();
	void manage_classes();
	void statistics();
private:
	void get_response()
	{
		while (true) 
		{
			std::cin >> m_response;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
			}
			else
			{
				if (m_response == "q" || m_response == "exit" || m_response == "Q" || m_response == "quit") { m_stop = true; break; }
				break;
			}
		}
	}
	std::string get_response2()
	{
		std::string m_response;
		
		while (true)
		{
			std::cin >> m_response;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
			}
			else
			{
				if (m_response == "q" || m_response == "exit" || m_response == "Q" || m_response == "quit") { m_stop = true; break; }
				break;
			}
		}
		return m_response;
	}
	bool check_name(std::string a) {
		for (int i = 0; i < a.length(); ++i) {
			if (

				!(		(a[i] >= 'A' && a[i] <= 'Z')	||	 (a[i] >= 'a' && a[i] <= 'z')	)

				) {
				return false;
			}
			
				
			

		}
		return true;

	}
	bool check_PESEL(std::string a) {
		if (a.length() != 11) {
			return false;
		}

		for (int i = 0; i < a.length(); ++i) {
			if (!(a[i] >= '0' && a[i] <= '9'))
			{
				return false;
			}
				
			

		}
		return true;
	}
};

#endif // 
