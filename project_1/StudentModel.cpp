
#include <regex>
#include "StudentModel.h"

using namespace school;


bool StudentModel::add_student(const StudentInfo& student)
{
	if (!(std::regex_match(student.m_firstname, m_verify_name) && std::regex_match(student.m_lastname, m_verify_name)))
	{
		return false;
	}
	if (!std::regex_match(student.m_PESEL, m_verify_PESEL)) return false;
	return true;
}