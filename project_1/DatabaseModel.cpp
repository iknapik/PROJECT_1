#include "DatabaseModel.h"
#include <algorithm>

using namespace cheshire;
using namespace school;

DatabaseModel::DatabaseModel()
{
	m_classdao = std::make_unique<BaseDao<ClassInfo>>("classes_db.txt", CLASS_FIELD_NAMES);
	m_sdao = std::make_unique<BaseDao<StudentInfo>>("students_db.txt", STUDENT_FIELD_NAMES);
	m_profdao = std::make_unique<BaseDao<ProfessorInfo>>("professors_db.txt", PROFESSOR_FIELD_NAMES);
	m_markdao = std::make_unique<BaseDao<MarkInfo>>("marks_db.txt", MARK_FIELD_NAMES);

	m_marks_ptr = m_markdao->get_rows();
	m_professors_ptr = m_profdao->get_rows();
	m_classes_ptr = m_classdao->get_rows();
	m_students_ptr = m_sdao->get_rows();
	m_slookup = std::make_unique<std::vector<std::string>>();

	m_slookup->reserve(m_students_ptr->size());
	for (auto& pair : *m_students_ptr)
	{
		auto stream = std::ostringstream();
		auto vec = pair.second->to_string_vector();

		for (auto& i : vec)
		{
			stream << to_lower(i) << " ";
		}
		try
		{
			stream << to_lower(m_classes_ptr->at(pair.second->m_class_id)->m_name) << " ";
		}
		catch(const std::out_of_range& err){stream << "unassigned ";}
		stream << "--id=" << pair.first;
		m_slookup->push_back(stream.str());
	}
}

// getters for add/remove/update //
template <>
std::map<uint, std::unique_ptr<StudentInfo>>* DatabaseModel::get_ptr() const { return m_students_ptr.get(); }
template <>
std::map<uint, std::unique_ptr<ClassInfo>>* DatabaseModel::get_ptr() const { return m_classes_ptr.get(); }
template <>
std::map<uint, std::unique_ptr<ProfessorInfo>>* DatabaseModel::get_ptr() const { return m_professors_ptr.get(); }
template <>
std::map<uint, std::unique_ptr<MarkInfo>>* DatabaseModel::get_ptr() const { return m_marks_ptr.get(); }
template <>
BaseDao<ClassInfo>* DatabaseModel::get_dao_ptr() const { return m_classdao.get(); }
template <>
BaseDao<StudentInfo>* DatabaseModel::get_dao_ptr() const { return m_sdao.get(); }
template <>
BaseDao<ProfessorInfo>* DatabaseModel::get_dao_ptr() const { return m_profdao.get(); }
template <>
BaseDao<MarkInfo>* DatabaseModel::get_dao_ptr() const { return m_markdao.get(); }

template <class Info>
bool DatabaseModel::add(Info& info)
{
	uint id = get_dao_ptr<Info>()->add_row(info);
	if (id != 0)
	{
		info.set_id(id);
		auto obj = std::make_unique<Info>();
		*obj = info;
		get_ptr<Info>()->emplace(id, std::move(obj));
		return true;
	}
	return false;
}
template bool DatabaseModel::add(StudentInfo& info);
template bool DatabaseModel::add(ClassInfo& info);
template bool DatabaseModel::add(ProfessorInfo& info);
template bool DatabaseModel::add(MarkInfo& info);

template <class Info>
bool DatabaseModel::update(const Info& info)
{
	if (get_dao_ptr<Info>()->update_row(info))
	{
		*(get_ptr<Info>()->at(info.get_id())) = info;
		return true;
	}
	return false;
}
template bool DatabaseModel::update<StudentInfo>(const StudentInfo& info);
template bool DatabaseModel::update<ClassInfo>(const ClassInfo& info);
template bool DatabaseModel::update<ProfessorInfo>(const ProfessorInfo& info);

template <class Info>
bool DatabaseModel::remove(unsigned id)
{
	if (get_dao_ptr<Info>()->remove_row(id))
	{
		get_ptr<Info>()->erase(id);
		return true;
	}
	return false;
}
template bool DatabaseModel::remove<StudentInfo>(unsigned id);
template bool DatabaseModel::remove<ClassInfo>(unsigned id);
template bool DatabaseModel::remove<ProfessorInfo>(unsigned id);

template <class Info>
Info DatabaseModel::get_by_id(unsigned id) const
{	
		return *(get_ptr<Info>()->at(id));
}
template StudentInfo DatabaseModel::get_by_id(unsigned id) const;
template ClassInfo DatabaseModel::get_by_id(unsigned id) const;
template ProfessorInfo DatabaseModel::get_by_id(unsigned id) const;

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
	std::string str_lower = to_lower(str);

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

std::list<StudentInfo> DatabaseModel::get_students_by_class(const std::string& str) const
{
	std::list<StudentInfo> list{};
	if (str.empty()) return list;
	std::string str_lower = to_lower(str);
	unsigned class_id = 0;
	for (auto& pair : *m_classes_ptr)
	{
		if (str_lower == pair.second->m_name)
		{
			class_id = pair.first;
			break;
		}
	}
	if (class_id)
	{
		for (auto& pair : *m_students_ptr)
		{
			if (pair.second->m_class_id == class_id) list.push_back(*(pair.second));
		}
	}
	return list;
}
