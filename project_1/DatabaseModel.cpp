#include "DatabaseModel.h"
#include <algorithm>

using namespace cheshire;
using namespace school;

DatabaseModel::DatabaseModel()
{
	m_sdao = std::make_unique<BaseDao<StudentInfo>>("students_db.txt", STUDENT_FIELD_NAMES);
	m_students_ptr = m_sdao->get_rows();
	m_slookup = std::make_unique<std::vector<std::string>>();

	m_slookup->reserve(m_students_ptr->size());
	for (auto& pair : *m_students_ptr)
	{
		auto stream = std::ostringstream();
		auto vec = pair.second->to_string_vector();

		for (auto& i : vec)
		{
			for (auto &chr : i)
			{
				chr = tolower(chr);
			}
			stream << i << " ";
		}
		stream << "--id=" << pair.first;
		m_slookup->push_back(stream.str());
	}
}

std::list<StudentInfo> DatabaseModel::find_students(const std::string& str) const
{
	auto list = std::list<StudentInfo>();
	if (str.empty()) return list;

	std::istringstream stream(str);
	std::vector<std::string> result(std::istream_iterator<std::string>{stream}, std::istream_iterator<std::string>());
	
	if (result.size() == 1)
	{
		for (auto id : find_students_helper(str))
		{
			list.push_back(*(m_students_ptr->at(id)));
		}
		return list;
	}
	auto templist1 = find_students_helper(result[0]);
	std::set<unsigned> temp1{templist1.begin(), templist1.end()};

	for (auto it = result.begin() + 1; it != result.end(); ++it)
	{
		auto templist2 = find_students_helper(result[0]);
		std::set<unsigned> temp2{ templist2.begin(), templist2.end()};
		std::set_intersection(temp1.cbegin(), temp1.cend(), temp2.cbegin(), temp2.cend(), std::back_inserter(templist1));
		temp1 = std::set<unsigned>(templist1.begin(), templist1.end());
	}
	for (auto i : temp1)
	{
		list.push_back(*(m_students_ptr->at(i)));
	}
	return list;
	
}

std::list<unsigned> DatabaseModel::find_students_helper(const std::string& str) const
{
	std::string str_lower{""};
	for (auto &i : str)
	{
		str_lower += tolower(i);
	}
	auto list = std::list<unsigned>();

	for (auto& elem : *m_slookup)
	{
		if (elem.find(str_lower) != std::string::npos)
		{
			unsigned id = id_from_lookup_elem(elem);
			list.push_back(id);
		}
	}
	return list;
}

unsigned DatabaseModel::id_from_lookup_elem(const std::string& str) const
{
	int last = str.rfind("--id=");
	last += 5;

	return std::stoul(str.substr(last));
}

std::list<StudentInfo> DatabaseModel::get_students_by_ids(const std::set<uint>& ids) const
{
	auto list = std::list<StudentInfo>();
	if (ids.empty()) return list;
	for (auto id : ids)
	{
		try 
		{
			list.push_back(*(m_students_ptr->at(id)));
		}
		catch (const std::out_of_range& err) {}
	}
	return list;
	
}


bool DatabaseModel::add_student(const StudentInfo& student)
{ 
	uint id = m_sdao->add_row(student);
	if (id != 0) 
	{
		auto stud = std::make_unique<StudentInfo>();
		*stud = student;
		stud->set_id(id);
		m_students_ptr->emplace(id, std::move(stud));
		return true;
	}
	return false;
}

bool DatabaseModel::remove_student(unsigned id)
{
	if (m_sdao->remove_row(id)) 
	{ 
		m_students_ptr->erase(id); 
		return true;
	}
	return false;
}

bool DatabaseModel::update_student(const StudentInfo& student)
{
	if (m_sdao->update_row(student))
	{
		*(m_students_ptr->at(student.get_id())) = student;
		return true;
	}
	return false;
}

StudentInfo DatabaseModel::get_student_by_id(uint id) const
{
	try
	{
		return *(m_students_ptr->at(id));
	}
	catch (const std::out_of_range& err) 
	{
		return StudentInfo();
	}
}