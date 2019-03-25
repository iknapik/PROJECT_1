//DATA ACCESS OBJECT
// serializacja/deserialzacja

#ifndef STUDENTDAO_H_
#define STUDENTDAO_H_
#include "StudentInfo.h"
#include <memory>
#include <map>



namespace school {
	

class StudentDao {
	typedef unsigned int uint;
	const char* m_file_name;

	friend StudentInfo;
public:
	StudentDao(const char* filename = "studends_db.txt") : m_file_name(filename) {}
	bool add_student(const StudentInfo& student) const;
	bool remove_student(uint id) const;
	bool update_student(const StudentInfo& student) const;
	std::unique_ptr<std::map<uint, std::unique_ptr<StudentInfo>>> get_students() const;
	StudentInfo get_student(uint id) const;
};

}

#endif
