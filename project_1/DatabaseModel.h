
#ifndef DATABASETMODEL_H_
#define DATABASETMODEL_H_

#include "MarkInfo.h"
#include "StudentInfo.h"
#include "BaseDao.h"
#include "ClassInfo.h"
#include "ProfessorInfo.h"
#include <list>
#include "Errors.h"

namespace school {

	using namespace school;
	using namespace cheshire;

	class DatabaseModel
	{
		typedef unsigned uint;
		//database access objects
		std::unique_ptr<BaseDao<ClassInfo>> m_classdao;
		std::unique_ptr<BaseDao<StudentInfo>> m_sdao;
		std::unique_ptr<BaseDao<ProfessorInfo>> m_profdao;
		std::unique_ptr<BaseDao<MarkInfo>> m_markdao;

		//lookup containing student id, string with student data that helps when searching by phrase
		std::unique_ptr <std::map<uint, std::string>> m_slookup;
		//student id, ids of his marks 
		std::unique_ptr<std::unordered_map<uint, std::set<uint>>> m_mark_student_lookup;
		//profesors id, ids of marks given by him
		std::unique_ptr<std::unordered_map<uint, std::set<uint>>> m_mark_professor_lookup;
		
		//maps reflecting the state of database
		std::unique_ptr<std::unordered_map<uint, std::shared_ptr<StudentInfo>>> m_students_ptr;
		std::unique_ptr<std::unordered_map<uint, std::shared_ptr<ClassInfo>>> m_classes_ptr;
		std::unique_ptr<std::unordered_map<uint, std::shared_ptr<ProfessorInfo>>> m_professors_ptr;
		std::unique_ptr<std::unordered_map<uint, std::shared_ptr<MarkInfo>>> m_marks_ptr;

	public:
		explicit DatabaseModel();
		
		//it searches in all STUDENT_FIELD_NAMES and class names
		//it's case insensitive
		std::list<StudentInfo> find_students(const std::string& str) const;

		//if no students found these functions return empty list
		std::list<StudentInfo> get_students_by_ids(const std::set<uint>& ids) const;
		std::list<StudentInfo> get_students_by_class(const std::string& str) const;
		std::list<StudentInfo> get_students_by_class(uint id) const;
		
		//return readonly map that displays what's inside db
		template <class Info>
		const std::map<const uint,std::shared_ptr<const Info>> get_all()
		{
			std::map<const uint, std::shared_ptr<const Info>> map;
			for (auto& pair : *get_ptr<Info>())
			{	
				map.insert_or_assign(pair.first, pair.second);
			}
			return map;
		}

		
		//worst case these return empty list
		std::list<MarkInfo> get_marks_by_student_id(uint id) const;
		std::list<MarkInfo> get_marks_by_professor_id(uint id) const;

		//gets any Info class by id, you have to specify correct Type to use this function
		//throws InvalidID exception if id not found
		template <class Info>
		Info get_by_id(uint id) const;

		//with update/add just pass in Info class it will know what to do
		//updating/adding marks can throw two exceptions: InvalidStudentID or InvalidProfessorID
		//updating/adding students can throw: InvalidClassID
		template <class Info>
		bool update(const Info& info);
		template <class Info>
		bool add(Info& info);
		//with remove and get_by_id you have to explicitly select what template to use
		//return false if id not found
		//removing student removes all his marks
		//removing class may throw an error when there are still students in that class
		template <class Info>		
		bool remove_by_id(uint id);
		template <class Info>
		bool remove(Info& info)
		{
			return remove_by_id<Info>(info.get_id());
		}

		bool is_valid(const StudentInfo& info) const;
		//bool is_valid(const ClassInfo& info) const;
		//bool is_valid(const ProfessorInfo& info) const;
		bool is_valid(const MarkInfo& info) const;
	private:
		template <class Info>
		bool _update(const Info& info);
		template <class Info>
		bool _add(Info& info);
		template <class Info>		
		bool _remove(uint id);
		std::set<unsigned> find_students_helper(const std::string& str) const;
		//templated getter for table pointers
		template <class Info>
		std::unordered_map<uint, std::shared_ptr<Info>>* get_ptr() const;
		std::string make_lookup_str_from_student(const StudentInfo& stud) const;
		//templated getter for db pointers
		template <class Info>
		BaseDao<Info>* get_dao_ptr() const;
		
		std::string to_lower(const std::string& str) const
		{
			std::string temp{str};
			for (auto& chr : temp)
				chr = tolower(chr);
			return temp;
		}
	};




}

#endif
