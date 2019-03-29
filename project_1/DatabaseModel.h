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
		std::unique_ptr<BaseDao<StudentInfo>> m_sdao;
		std::unique_ptr<std::vector<std::string>> m_slookup;
		std::unique_ptr<std::map<unsigned, std::unique_ptr<StudentInfo>>> m_students_ptr;

	public:
		explicit DatabaseModel();
		std::list<StudentInfo> find_students(const std::string& str) const;
	private:
		std::list<unsigned> find_students_helper(const std::string& str) const;
		unsigned id_from_lookup_elem(const std::string& str) const;				
	};




}

#endif
