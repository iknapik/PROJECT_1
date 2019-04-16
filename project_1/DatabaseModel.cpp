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

	m_slookup = std::make_unique<std::map<unsigned, std::string>>();
	m_mark_student_lookup = std::make_unique < std::unordered_map<unsigned, std::set<unsigned>>>();
	m_mark_professor_lookup = std::make_unique < std::unordered_map<unsigned, std::set<unsigned>>>();
	
	for (auto& pair : *m_students_ptr)
	{
		m_slookup->emplace(pair.first, make_lookup_str_from_student(*pair.second));
	}
	for (auto& pair : *m_marks_ptr)
	{
		(*m_mark_student_lookup)[pair.second->m_student_id].insert(pair.first);
	}
	for (auto& pair : *m_marks_ptr)
	{
		(*m_mark_professor_lookup)[pair.second->m_professor_id].insert(pair.first);
	}
}

// getters for add/remove/update //
template <>
std::unordered_map<unsigned, std::shared_ptr<StudentInfo>>* DatabaseModel::get_ptr() const { return m_students_ptr.get(); }
template <>
std::unordered_map<unsigned, std::shared_ptr<ClassInfo>>* DatabaseModel::get_ptr() const { return m_classes_ptr.get(); }
template <>
std::unordered_map<unsigned, std::shared_ptr<ProfessorInfo>>* DatabaseModel::get_ptr() const { return m_professors_ptr.get(); }
template <>
std::unordered_map<unsigned, std::shared_ptr<MarkInfo>>* DatabaseModel::get_ptr() const { return m_marks_ptr.get(); }
template <>
BaseDao<ClassInfo>* DatabaseModel::get_dao_ptr() const { return m_classdao.get(); }
template <>
BaseDao<StudentInfo>* DatabaseModel::get_dao_ptr() const { return m_sdao.get(); }
template <>
BaseDao<ProfessorInfo>* DatabaseModel::get_dao_ptr() const { return m_profdao.get(); }
template <>
BaseDao<MarkInfo>* DatabaseModel::get_dao_ptr() const { return m_markdao.get(); }

template<>
bool DatabaseModel::add(StudentInfo& info)
{
	is_valid(info);
	if (_add(info))
	{
		m_slookup->emplace(info.get_id(), make_lookup_str_from_student(info));
		return true;
	}
	return false;
}

template<>
bool DatabaseModel::add(MarkInfo& info)
{
	is_valid(info);
	if (_add(info))
	{
		(*m_mark_student_lookup)[info.m_student_id].insert(info.get_id());
		(*m_mark_professor_lookup)[info.m_professor_id].insert(info.get_id());
		return true;
	}
	return false;
}

template<class Info>
bool DatabaseModel::add(Info& info)
{
	if (_add(info))
	{		
		return true;
	}
	return false;
}

template bool DatabaseModel::add(ClassInfo&);
template bool DatabaseModel::add(ProfessorInfo&);

template <class Info>
bool DatabaseModel::_add(Info& info)
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
template bool DatabaseModel::_add(StudentInfo&);
template bool DatabaseModel::_add(ClassInfo&);
template bool DatabaseModel::_add(ProfessorInfo&);
template bool DatabaseModel::_add(MarkInfo&);

template<>
bool DatabaseModel::update(const StudentInfo& info)
{
	is_valid(info);
	if (_update(info))
	{
		m_slookup->at(info.get_id()) = make_lookup_str_from_student(info);
		return true;
	}
	return false;
}

template<>
bool DatabaseModel::update(const MarkInfo& info)
{
	is_valid(info);
	auto old_mark = *(m_marks_ptr->at(info.get_id()));
	if (_update(info))
	{
		if (old_mark.m_student_id != info.m_student_id)
		{
			m_mark_student_lookup->at(old_mark.m_student_id).erase(info.get_id());
			m_mark_student_lookup->at(info.m_student_id).insert(info.get_id());
		}
		if (old_mark.m_professor_id != info.m_professor_id)
		{
			m_mark_professor_lookup->at(old_mark.m_professor_id).erase(info.get_id());
			m_mark_professor_lookup->at(info.m_professor_id).insert(info.get_id());
		}
		return true;
	}
	return false;
}
template <class Info>
bool DatabaseModel::update(const Info& info)
{
	return _update(info);
}
template bool DatabaseModel::update(const ClassInfo&);
template bool DatabaseModel::update(const ProfessorInfo&);

template <class Info>
bool DatabaseModel::_update(const Info& info)
{
	if (get_dao_ptr<Info>()->update_row(info))
	{
		*(get_ptr<Info>()->at(info.get_id())) = info;
		return true;
	}
	return false;
}
template bool DatabaseModel::_update<StudentInfo>(const StudentInfo&);
template bool DatabaseModel::_update<ClassInfo>(const ClassInfo&);
template bool DatabaseModel::_update<ProfessorInfo>(const ProfessorInfo&);

template <>
bool DatabaseModel::remove<StudentInfo>(uint id)
{
	if (_remove<StudentInfo>(id))
	{
		m_slookup->erase(id);
		return true;
	}
	return false;
}
template <>
bool DatabaseModel::remove<MarkInfo>(uint id)
{
	MarkInfo old_mark;
	try
	{
		old_mark = *m_marks_ptr->at(id);
	}
	catch (const std::out_of_range&) 
	{
		return false;
	}

	if (_remove<MarkInfo>(id))
	{
		m_mark_professor_lookup->at(old_mark.m_professor_id).erase(id);
		m_mark_student_lookup->at(old_mark.m_student_id).erase(id);
		return true;
	}
	return false;
}

template <class Info>
bool DatabaseModel::remove(unsigned id)
{
	return _remove<Info>(id);
}
template bool DatabaseModel::remove<ClassInfo>(unsigned);
template bool DatabaseModel::remove<ProfessorInfo>(unsigned);

template <class Info>
bool DatabaseModel::_remove(unsigned id)
{
	if (get_dao_ptr<Info>()->remove_row(id))
	{
		get_ptr<Info>()->erase(id);
		return true;
	}
	return false;
}
template bool DatabaseModel::_remove<StudentInfo>(unsigned);
template bool DatabaseModel::_remove<ClassInfo>(unsigned);
template bool DatabaseModel::_remove<ProfessorInfo>(unsigned);

template <class Info>
Info DatabaseModel::get_by_id(unsigned id) const
{
	try
	{
		return *(get_ptr<Info>()->at(id));
	}
	catch (const std::out_of_range&) 
	{
		throw InvalidField(ErrorCode::INVALID_ID, id);
	}	
}
template StudentInfo DatabaseModel::get_by_id(unsigned) const;
template ClassInfo DatabaseModel::get_by_id(unsigned) const;
template ProfessorInfo DatabaseModel::get_by_id(unsigned) const;

std::list<StudentInfo> DatabaseModel::find_students(const std::string& str) const
{
	auto list = std::list<StudentInfo>();
	if (str.empty()) return list;
	std::string lower_str = to_lower(str);
	std::istringstream stream(lower_str);
	std::vector<std::string> result(std::istream_iterator<std::string>{stream}, std::istream_iterator<std::string>());
	for (auto& pair : *m_slookup)
	{
		bool match = true;
		for (auto& str : result)
		{
			if (pair.second.find(str) == std::string::npos) { match = false;  break; }
		}
		if (match)
		{
			list.push_back(*(m_students_ptr->at(pair.first)));
		}
	}
	return list;
	
}


std::list<StudentInfo> DatabaseModel::get_students_by_ids(const std::set<uint>& ids) const
{
	auto list = std::list<StudentInfo>();
	if (ids.empty()) return list;
	for (auto id : ids)
	{
		if (m_students_ptr->count(id))	list.push_back(*(m_students_ptr->at(id)));

	}
	return list;
	
}
std::list<StudentInfo> DatabaseModel::get_students_by_class(uint id) const
{
	std::list<StudentInfo> list;
	for (auto& pair : *m_students_ptr)
	{
		if (pair.second->m_class_id == id)
		{
			list.push_back(*pair.second);
		}
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
		return get_students_by_class(class_id);
	}
	return list;
}

std::string DatabaseModel::make_lookup_str_from_student(const StudentInfo& stud) const
{
	auto stream = std::ostringstream();
	auto vec = stud.to_string_vector();
	for (auto& i : vec)
	{
		stream << to_lower(i) << " ";
	}
	try
	{
		stream << to_lower(m_classes_ptr->at(stud.get_class_id())->m_name) << " ";
	}
	catch (const std::out_of_range&) { stream << "unassigned "; }
	stream << "--id=" << stud.get_id();
	return stream.str();
}

std::list<MarkInfo> DatabaseModel::get_marks_by_student_id(uint id) const
{
	std::list<MarkInfo> list;
		for (auto i : m_mark_student_lookup->at(id))
		{
			if (m_marks_ptr->count(i)) list.push_back(*(m_marks_ptr->at(i)));
		}

	return list;
}
std::list<MarkInfo> DatabaseModel::get_marks_by_professor_id(uint id) const
{
	std::list<MarkInfo> list;
	
		for (auto i : m_mark_professor_lookup->at(id))
		{
			if (m_marks_ptr->count(i)) list.push_back(*(m_marks_ptr->at(i)));
		}

	return list;
}

bool DatabaseModel::is_valid(const StudentInfo& info) const
{
	if (!m_classes_ptr->count(info.m_class_id))				throw InvalidField(ErrorCode::INVALID_CLASS_ID, info.m_class_id);
	return true;
}

bool DatabaseModel::is_valid(const MarkInfo& info) const
{
	if (!m_students_ptr->count(info.m_student_id))	throw InvalidField(ErrorCode::INVALID_STUDENT_ID, info.m_student_id);
	
	if (!m_professors_ptr->count(info.m_professor_id)) throw InvalidField(ErrorCode::INVALID_PROFESSOR_ID, info.m_professor_id);

	return true;
	
}