//DATA ACCESS OBJECT
// serializacja/deserialzacja

#ifndef STUDENTDAO_H_
#define STUDENTDAO_H_
#include "StudentInfo.h"
#include "CSVDb.h"
#include <memory>
#include <map>
//#include <array>

const int number_of_student_fields = 6;

namespace school {
	

class StudentDao {
	typedef unsigned int uint;

	const std::vector<std::string> m_field_names{"FIRSTNAME", "LASTNAME", "PESEL", "CITY", "ADDRESS"};
	const char* m_file_name;
	std::unique_ptr<cheshire::CSVDb> m_database;

	std::unique_ptr<std::map<const std::string, std::string>> convert(const StudentInfo& sinfo) const;
	std::unique_ptr<StudentInfo> convert(uint id, const std::unique_ptr<std::map<const std::string, std::string>>& data) const;
public:
	explicit StudentDao(const char* filename = "students_db.txt") : m_file_name(filename), m_database(std::make_unique<cheshire::CSVDb>(filename, m_field_names)) {}
	bool add_student(const StudentInfo& student) const;
	bool remove_student(uint id) const;
	bool update_student(const StudentInfo& student) const;
	std::unique_ptr<std::map<uint, std::unique_ptr<StudentInfo>>> get_students() const;
	std::unique_ptr<StudentInfo> get_student(uint id) const;
};

}

#endif
