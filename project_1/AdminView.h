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
};

#endif // 
