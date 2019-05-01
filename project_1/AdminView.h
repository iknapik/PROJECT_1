#ifndef ADMIN_VIEW_H_
#define ADMIN_VIEW_H_
#include "DatabaseModel.h"
#include <iostream>

class AdminView
{
	school::DatabaseModel& m_db;
	bool stop = false;
public:
	explicit AdminView(school::DatabaseModel& database) : m_db(database) {}
	void menu();
	void manage_passwords();
	void manage_students();
	void manage_professors();
	void manage_marks();
	void manage_classes();
	void statistics();
	bool is_exit_requested() { return stop; }
};

#endif // 
