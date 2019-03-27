
#include "StudentDao.h"

#include <fstream>
#include <string>
#include <cstdio>
/*
using namespace school;
std::array<std::string, number_of_student_fields> StudentDao::convert(const StudentInfo& sinfo) const
{
	auto arr = std::array<std::string, number_of_student_fields>();
	arr[0] = std::to_string(sinfo.get_id());
	arr[1] = sinfo.m_firstname;
	arr[2] = sinfo.m_lastname;
	arr[3] = sinfo.m_PESEL;
	arr[4] = sinfo.m_city;
	arr[5] = sinfo.m_address;
	return arr;
}

std::unique_ptr<StudentInfo> StudentDao::convert(const std::array<std::string, number_of_student_fields>& arr) const
{
	auto student_ptr = std::make_unique<StudentInfo>(arr[1], arr[2], arr[3], arr[4], arr[5], std::stoul(arr[0]));
	return student_ptr;
}

bool StudentDao::add_student(const StudentInfo& student) const
{
	return m_database->add_row(convert(student));
}

bool StudentDao::remove_student(unsigned id) const
{
	return m_database->remove_row(id);
}

bool StudentDao::update_student(const StudentInfo& sinfo) const
{
	return m_database->update_row(convert(sinfo));
}

std::unique_ptr<std::map<unsigned, std::unique_ptr<StudentInfo>>> StudentDao::get_students() const
{
	auto map_ptr = std::make_unique<std::map<unsigned, std::unique_ptr<StudentInfo>>>();
	auto list_ptr = m_database->get_rows();
	for (auto& i : *list_ptr)
	{
		auto student = convert(*i);
		(*map_ptr)[student->get_id()] = std::move(student);
	}
	return map_ptr;
}

std::unique_ptr<StudentInfo> StudentDao::get_student(unsigned id) const
{
	return convert(m_database->get_row(id));
}
*/