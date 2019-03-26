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
	const char* m_file_name;	
	std::unique_ptr<cheshire::CSVDb<number_of_student_fields>> m_database;
	friend StudentInfo;
	std::array<std::string, number_of_student_fields> convert(const StudentInfo& sinfo) const;
	std::unique_ptr<StudentInfo> convert(const std::array<std::string, number_of_student_fields>& arr) const;
public:
	constexpr explicit StudentDao(const char* filename = "students_db.txt") : m_file_name(filename), m_database(std::make_unique< cheshire::CSVDb<number_of_student_fields>>(filename)) {}
	bool add_student(const StudentInfo& student) const;
	bool remove_student(uint id) const;
	bool update_student(const StudentInfo& student) const;
	std::unique_ptr<std::map<uint, std::unique_ptr<StudentInfo>>> get_students() const;
	std::unique_ptr<StudentInfo> get_student(uint id) const;
};

}

#endif
