#ifndef STUDENT_VIEW_H_
#define STUDENT_VIEW_H_
#include "DatabaseModel.h"
#include "StudentInfo.h"
class StudentView

{
	//const project::StudentInfo m_sinfo;
	typedef unsigned uint;
	const project::DatabaseModel& m_db;
    bool m_stop = false;
    std::string m_response;
    uint id;
public:
    StudentView(const project::DatabaseModel& db, uint id) : m_db(db), id(id){};
    
    unsigned get_id_from_pesel(const project::DatabaseModel& db, const std::string& str);
	void menu();
    void marks_view();
	void personal_information();
	void subject_view();
    std::string zmiana(std::string& ciag);
    bool is_exit_requested(std::string str)
    {
        if (str == "Q" || str == "EXIT" || str == "QUIT") { m_stop = true; return true; }
        return false;
    }
    
    bool get_stop()const
    
    {
        return m_stop;
    }
    
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
            {   m_response=zmiana(m_response);
                is_exit_requested(m_response);
            
                break;
            }
        }
    }
};

#endif