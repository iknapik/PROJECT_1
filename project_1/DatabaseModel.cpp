#include "DatabaseModel.h"
#include <algorithm>
#include <iterator>

using namespace cheshire;
using namespace project;

DatabaseModel::DatabaseModel(const std::string& class_db_name, const std::string& stud_db_name, const std::string& prof_db_name, const std::string& mark_db_name)
{

	m_classdao = std::make_unique<BaseDao<ClassInfo>>(class_db_name.c_str(), CLASS_FIELD_NAMES);
	m_sdao = std::make_unique<BaseDao<StudentInfo>>(stud_db_name.c_str(), STUDENT_FIELD_NAMES);
	m_profdao = std::make_unique<BaseDao<ProfessorInfo>>(prof_db_name.c_str(), PROFESSOR_FIELD_NAMES);
	m_markdao = std::make_unique<BaseDao<MarkInfo>>(mark_db_name.c_str(), MARK_FIELD_NAMES);

	m_marks_ptr = m_markdao->get_rows();
	m_professors_ptr = m_profdao->get_rows();
	m_classes_ptr = m_classdao->get_rows();
	m_students_ptr = m_sdao->get_rows();

	m_slookup = std::make_unique<std::map<uint, std::string>>();
	m_mark_student_lookup = std::make_unique < std::unordered_map<uint, std::set<uint>>>();
	m_mark_professor_lookup = std::make_unique < std::unordered_map<uint, std::set<uint>>>();
	
	auto file = std::ifstream("passwords.txt", std::ios::in);
	if (file.fail()) { m_admin_password = "admin"; m_professor_password = "professor"; m_student_password = "student"; save_password(); }
	std::getline(file, m_admin_password, ',');
	std::getline(file, m_professor_password, ',');
	std::getline(file, m_student_password, ',');
	file.close();

	for (auto& pair : *m_students_ptr)
	{
		m_slookup->emplace(pair.first, make_lookup_str_from_student(*pair.second));
		m_mark_student_lookup->emplace(pair.first, std::set<uint>());
	}
	for (auto& pair : *m_professors_ptr)
	{
		m_mark_professor_lookup->emplace(pair.first, std::set<uint>());
	}
	m_mark_professor_lookup->emplace(0, std::set<uint>());
	for (auto& pair : *m_marks_ptr)
	{
		m_mark_student_lookup->at(pair.second->m_student_id).insert(pair.first);
		m_mark_professor_lookup->at(pair.second->m_professor_id).insert(pair.first);
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
void DatabaseModel::add(StudentInfo& info)
{
	is_valid(info);
	if (_add(info))
	{
		m_slookup->emplace(info.get_id(), make_lookup_str_from_student(info));
		m_mark_student_lookup->emplace(info.get_id(), std::set<uint>{});
		return;
	}
	throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);
}

template<>
void DatabaseModel::add(MarkInfo& info)
{
	is_valid(info);
	if (_add(info))
	{
		m_mark_student_lookup->at(info.m_student_id).insert(info.get_id());
		m_mark_professor_lookup->at(info.m_professor_id).insert(info.get_id());
		return;
	}
	throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);

}
template <>
void DatabaseModel::add(ProfessorInfo& info)
{
	is_valid(info);
	if (_add(info))
	{		
		m_mark_professor_lookup->emplace(info.get_id(), std::set<uint>{});
		return;
	}
	throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);
}

template<class Info>
void DatabaseModel::add(Info& info)
{
	if (_add(info))
	{		
		return;
	}
	throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);
}

template void DatabaseModel::add(ClassInfo&);
template void DatabaseModel::add(ProfessorInfo&);

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

void DatabaseModel::update(const StudentInfo& info)
{
	if (!m_slookup->count(info.get_id())) throw DatabaseError(ErrorCode::INVALID_STUDENT_ID);
	is_valid(info);
	if (_update(info))
	{
		m_slookup->at(info.get_id()) = make_lookup_str_from_student(info);
		return;
	}
	throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);
}

void DatabaseModel::update(const MarkInfo& info)
{
	if (!m_marks_ptr->count(info.get_id())) throw DatabaseError(ErrorCode::INVALID_MARK_ID);

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
		return;
	}
	throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);
}
void DatabaseModel::update(const ClassInfo& info)
{
	if (!m_classes_ptr->count(info.get_id())) throw DatabaseError(ErrorCode::INVALID_CLASS_ID);
	if (!_update(info)) throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);
}

void DatabaseModel::update(const ProfessorInfo& info)
{
	 
	is_valid(info);
	if (!m_professors_ptr->count(info.get_id())) throw DatabaseError(ErrorCode::INVALID_PROFESSOR_ID);
	if (!_update(info)) throw DatabaseError(ErrorCode::DISSALOWED_CHARACTER);
}

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


bool DatabaseModel::remove_mark_by_id(uint id)
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

bool DatabaseModel::remove_student_by_id(uint id)
{
	if (m_students_ptr->count(id))
	{
		m_slookup->erase(id);
		auto marks = get_marks_by_student_id(id);
		for (auto& mark : marks)
		{
			remove_mark_by_id(mark.get_id());
		}
		m_mark_student_lookup->erase(id);
		return _remove<StudentInfo>(id);
	}
	return false;
}

bool DatabaseModel::remove_class_by_id(unsigned id)
{
	if (!m_classes_ptr->count(id)) return false;
	if (get_students_by_class(m_classes_ptr->at(id)->m_name).empty())
		return _remove<ClassInfo>(id);
	else
		throw DatabaseError(ErrorCode::CLASS_IN_USE, id);
}
bool DatabaseModel::remove_professor_by_id(unsigned id)
{
	if (m_professors_ptr->count(id))
	{
		auto mlist = get_marks_by_professor_id(id);
		for (auto& m : mlist)
		{
			m.m_professor_id = 0;
			update(m);
		}
		m_mark_professor_lookup->erase(id);
		return _remove<ProfessorInfo>(id);
	}
	return false;
}

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
		throw DatabaseError(ErrorCode::INVALID_ID, id);
	}	
}
template StudentInfo DatabaseModel::get_by_id(unsigned) const;
template ClassInfo DatabaseModel::get_by_id(unsigned) const;
template ProfessorInfo DatabaseModel::get_by_id(unsigned) const;
template MarkInfo DatabaseModel::get_by_id(unsigned) const;

std::list<StudentInfo> DatabaseModel::find_students(const std::string& str) const
{
	auto list = std::list<StudentInfo>();
	if (str.empty()) return list;
	std::string lower_str = trim(to_lower(str));
	if (lower_str.empty()) return list;
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
		if (str_lower == to_lower(pair.second->m_name))
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
	if (!m_students_ptr->count(id)) return list;
		for (auto i : m_mark_student_lookup->at(id))
		{
			if (m_marks_ptr->count(i)) list.push_back(*(m_marks_ptr->at(i)));
		}

	return list;
}
std::list<MarkInfo> DatabaseModel::get_marks_by_professor_id(uint id) const
{
	std::list<MarkInfo> list;
	if (!m_professors_ptr->count(id)) return list;
		for (auto i : m_mark_professor_lookup->at(id))
		{
			if (m_marks_ptr->count(i)) list.push_back(*(m_marks_ptr->at(i)));
		}

	return list;
}

void DatabaseModel::is_valid(const StudentInfo& info) const
{
	if (!m_classes_ptr->count(info.m_class_id))				throw DatabaseError(ErrorCode::INVALID_CLASS_ID, info.m_class_id);
	else if (!is_valid_PESEL(info.m_PESEL)) throw DatabaseError(ErrorCode::INVALID_PESEL, info.get_id());
}


void DatabaseModel::is_valid(const ProfessorInfo& info) const
{
	if (!is_valid_PESEL(info.m_PESEL)) throw DatabaseError(ErrorCode::INVALID_PESEL, info.get_id());
}

void DatabaseModel::is_valid(const MarkInfo& info) const
{
	//if student doesnt exists
	if (!m_students_ptr->count(info.m_student_id))	throw DatabaseError(ErrorCode::INVALID_STUDENT_ID, info.m_student_id);
	
	//if prof id == 0 it means professor was removed
	if (info.m_professor_id == 0) return;
	if (!m_professors_ptr->count(info.m_professor_id)) throw DatabaseError(ErrorCode::INVALID_PROFESSOR_ID, info.m_professor_id);
}

bool DatabaseModel::save_password() const
{

	if (m_admin_password.find(',') != std::string::npos || m_professor_password.find(',') != std::string::npos || m_student_password.find(',') != std::string::npos)
		return false;
	auto file = std::ofstream("passwords.txt", std::ios::out);
	file << m_admin_password << "," << m_professor_password << "," << m_student_password << ",";
	return true;
}

bool DatabaseModel::check_integrity() const
{
	auto slook = m_slookup->size();
	auto markslook = m_mark_student_lookup->size();
	auto markplook = m_mark_professor_lookup->size();
	auto studs = m_students_ptr->size();
	auto profs = m_professors_ptr->size();
	auto clss = m_classes_ptr->size();
	auto marks = m_marks_ptr->size();
	size_t total_marks = 0;
	size_t total_marks2 = 0;
	for (auto& pair : *m_mark_student_lookup)
	{
		total_marks += pair.second.size();
	}
	for (auto& pair : *m_mark_professor_lookup)
	{
		total_marks2 += pair.second.size();
	}
	/*
	std::cout << slook << " m_slookup size\n";
	std::cout << markslook << " m_mark_student size\n";
	std::cout << markplook << " m_mark_professor size\n";
	std::cout << studs << " stud_ptr\n";
	std::cout << profs << " prof_ptr\n";
	std::cout << clss << " classes_ptr\n";
	std::cout << marks << " marks_ptr\n";
	std::cout << total_marks << " marks counted from markstudentlookup\n";
	std::cout << total_marks2 << " marks counted from marksproflookup\n";
	*/
	return slook == studs && markslook == studs && markplook == profs + 1 &&
		total_marks == total_marks2;


}
bool DatabaseModel::is_valid_PESEL(const std::string_view& strv) const
{
	if (strv.size() == 11)
	{
		for (auto& chr : strv)
		{
			if (!isdigit(chr)) return false;
		}
		return true;
	}
	return false;
}