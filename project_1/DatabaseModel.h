//główna klasa do dostępu do bazy
//cała logika będzie tutaj
//obliczanie średnich, dostęp do danych
// przykładowo aby dodać studenta do bazy używamy
// student_model_instance.m_student_dao.add_student(...);

#ifndef DATABASETMODEL_H_
#define DATABASETMODEL_H_

#include "StudentInfo.h"
#include "BaseDao.h"
#include "ClassInfo.h"
#include <regex>
#include <list>

namespace school {

	using namespace school;
	using namespace cheshire;

	class DatabaseModel
	{
		typedef unsigned uint;

		std::unique_ptr<BaseDao<StudentInfo>> m_sdao;
		std::unique_ptr<std::vector<std::string>> m_slookup;
		std::unique_ptr<std::map<uint, std::unique_ptr<StudentInfo>>> m_students_ptr;

	public:
		explicit DatabaseModel();
		std::list<StudentInfo> find_students(const std::string& str) const;
		bool add_student(const StudentInfo& student);
		bool remove_student(uint id);
		bool update_student(const StudentInfo& student);
		StudentInfo get_student_by_id(uint id) const;
		std::list<StudentInfo> get_students_by_ids(const std::set<uint>& ids) const;
		std::list<StudentInfo> get_students_by_class(const std::string& str) const;

	private:
		std::list<unsigned> find_students_helper(const std::string& str) const;
		unsigned id_from_lookup_elem(const std::string& str) const;
	};




}

#endif
