
#ifndef DATABASETMODEL_H_
#define DATABASETMODEL_H_

#include "MarkInfo.h"
#include "StudentInfo.h"
#include "BaseDao.h"
#include "ClassInfo.h"
#include "ProfessorInfo.h"
#include <list>
#include "Errors.h"

namespace project {

	using namespace project;
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

		//three passwords one for admin, professors and student
		std::string m_admin_password;
		std::string m_professor_password;
		std::string m_student_password;

	public:
		explicit DatabaseModel(const std::string& class_db_name = "classes_db.txt", 
			const std::string& stud_db_name = "students_db.txt", 
			const std::string& prof_db_name = "professors_db.txt",
			const std::string& mark_db_name = "marks_db.txt");
		std::string get_admin_password() const {return m_admin_password;}
		std::string get_professor_password() const {return m_professor_password;}
		std::string get_student_password() const {return m_student_password;}

		// returns flase if passwords contains ','
		bool set_admin_password(const std::string& password) {m_admin_password = password; return save_password();}
		bool set_professor_password(const std::string& password) {m_professor_password = password; return save_password();}
		bool set_student_password(const std::string& password) {m_student_password = password; return save_password();}

		//it searches in all STUDENT_FIELD_NAMES and class names
		//it's case insensitive
		std::list<StudentInfo> find_students(const std::string& str) const;

		//if no students found these functions return empty list
		std::list<StudentInfo> get_students_by_ids(const std::set<uint>& ids) const;
		std::list<StudentInfo> get_students_by_class(const std::string& str) const;
		std::list<StudentInfo> get_students_by_class(uint id) const;
		
		//return readonly map that displays what's inside db
		template <class Info>
		const std::map<const uint,std::shared_ptr<const Info>> get_all() const
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
		//throws DatabaseError InvalidID exception if id not found
		template <class Info>
		Info get_by_id(uint id) const;

		//updating/adding classes can throw DatabaseError exception
		//',' is a dissalowed character ErrorCode::DISALOWED_CHARACTER
		//updating throws DatabaseError if item doesn't exists
		//when updating/adding student make sure it has proper class id
		//when updating/adding mark make sure it has proper student and professor id
		void update(const StudentInfo& info); //throws DatabaseError with errorcode:  ErrorCode::INVALID_STUDENT_ID, ErrorCode::DISSALOWED_CHARACTER, ErrorCode::INVALID_CLASS_ID
		void update(const ClassInfo& info); //throws DatabaseError with errorcode:  ErrorCode::INVALID_CLASS_ID, ErrorCode::DISSALOWED_CHARACTER
		void update(const ProfessorInfo& info); //throws DatabaseError with errorcode:  ErrorCode::INVALID_PROFESSOR_ID, ErrorCode::DISSALOWED_CHARACTER
		void update(const MarkInfo& info); //throws DatabaseError with errorcode:  ErrorCode::INVALID_STUDENT_ID, ErrorCode::DISSALOWED_CHARACTER, ErrorCode::INVALID_PROFESSOR_ID, ErrorCode::INVALID_MARK_ID
		
		//adding modifies info object to set correct id
		//so after adding you can use added object to get id info.get_id()
		template <class Info>
		void add(Info& info);
		
		//with remove and get_by_id you have to explicitly select what template to use
		//return false if id not found
		//removing student removes all his marks
		//removing class may throw an error when there are still students in that class
		bool remove_student_by_id(uint id);
		bool remove_class_by_id(uint id);
		bool remove_professor_by_id(uint id);
		bool remove_mark_by_id(uint id);

		bool remove_student(StudentInfo& student) {	return remove_student_by_id(student.get_id());}
		bool remove_class(ClassInfo& cls) {	return remove_class_by_id(cls.get_id());}
		bool remove_professor(ProfessorInfo& prof) {return remove_professor_by_id(prof.get_id());}
		bool remove_mark(MarkInfo& mark) {	return remove_mark_by_id(mark.get_id());}
		bool check_integrity() const;	
private:
		//these throw errors if infos are not valid
		void is_valid(const StudentInfo& info) const;
		//bool is_valid(const ClassInfo& info) const;
		void is_valid(const ProfessorInfo& info) const;
		void is_valid(const MarkInfo& info) const;
		bool is_valid_PESEL(const std::string_view& strv) const;
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
		bool save_password() const;
		
		std::string to_lower(const std::string& str) const
		{
			std::string temp{str};
			for (auto& chr : temp)
				chr = tolower(chr);
			return temp;
		}
		std::string trim(const std::string& str, char chr = ' ') const
		{		
			auto beg = str.begin();
			auto end = str.end() - 1;
			while (beg != end)
			{
				if (*beg == chr) ++beg;
				if (end == beg) break;
				if (*end == chr) --end;
				if (*beg != chr && *end != chr) break;
			}
			if (end == beg && chr == *beg) return std::string();
			return std::string(beg, end + 1);
		}
		
	};




}

#endif
