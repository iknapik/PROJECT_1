#ifndef STUDENT_VIEW_H_
#define STUDENT_VIEW_H_
#include "DatabaseModel.h"

class StudentView

{
	const school::StudentInfo m_sinfo;
	const school::DatabaseModel& m_db;
	bool stop = false;
public:
	StudentView(const school::DatabaseModel& db, unsigned id) : m_db(db), m_sinfo(db.get_by_id<school::StudentInfo>(id)) {}
	void menu();
	void marks_view();
	void personal_information();
	void subject_view();
};

#endif
