//DATA ACCESS OBJECT
// serializacja/deserialzacja

#ifndef STUDENTDAO_H_
#define STUDENTDAO_H_
#include "StudentInfo.h"
#include "CSVDb.h"
#include <memory>
#include <map>
//#include <array>

//extern const std::vector<std::string> school::FIELD_NAMES;

namespace school {
	

class StudentDao {
	typedef unsigned int uint;

	//const std::vector<std::string> m_field_names = FIEL;
	const char* m_file_name;
	std::unique_ptr<cheshire::CSVDbID> m_database;

	//std::unique_ptr<std::map<const std::string, std::string>> convert(const StudentInfo& sinfo) const;
	//std::unique_ptr<StudentInfo> convert(uint id, const std::unique_ptr<std::map<const std::string, std::string>>& data) const;
public:
	explicit StudentDao(const char* filename = "students_db.txt") : m_file_name(filename), m_database(std::make_unique<cheshire::CSVDbID>(filename, school::FIELD_NAMES)) {}
	bool add_student(const StudentInfo& student) const
	{
		return m_database->add_row(student.to_string_vector());
	}
	bool remove_student(uint id) const
	{
		return m_database->remove_row(id);
	}
	bool update_student(const StudentInfo& student) const
	{
		return m_database->update_row(student.get_id(), student.to_string_vector());
	}

	std::unique_ptr<std::map<uint, std::unique_ptr<StudentInfo>>> get_students() const
	{
		auto out_ptr = std::make_unique<std::map<uint, std::unique_ptr<StudentInfo>>>();
		auto rows_ptr = m_database->get_rows();
		for (auto &map_ptr : *rows_ptr)
		{
			auto sinfo_ptr = std::make_unique<StudentInfo>();
			sinfo_ptr->from_map(map_ptr.first, std::move(map_ptr.second));
			
			out_ptr->emplace(map_ptr.first, std::move(sinfo_ptr));
		}

		return out_ptr;
	}

	std::unique_ptr<StudentInfo> get_student(uint id) const
	{
		auto pair = m_database->get_row(id);
		auto ptr = std::make_unique<StudentInfo>();
		if (!pair.second->empty()) ptr->from_map(id, std::move(pair.second));
		return ptr;
	}
};

}

#endif
