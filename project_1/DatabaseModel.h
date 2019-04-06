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
#include "ProfessorInfo.h"
#include <regex>
#include <list>

namespace school {

	using namespace school;
	using namespace cheshire;

	class DatabaseModel
	{
		typedef unsigned uint;
		std::unique_ptr<BaseDao<ClassInfo>> m_classdao;
		std::unique_ptr<BaseDao<StudentInfo>> m_sdao;
		std::unique_ptr<BaseDao<ProfessorInfo>> m_profdao;

		std::unique_ptr<std::vector<std::string>> m_slookup;

		std::unique_ptr<std::map<uint, std::unique_ptr<StudentInfo>>> m_students_ptr;
		std::unique_ptr<std::map<uint, std::unique_ptr<ClassInfo>>> m_classes_ptr;
		std::unique_ptr<std::map<uint, std::unique_ptr<ProfessorInfo>>> m_professors_ptr;

	public:
		explicit DatabaseModel();
		
		//it searches in all STUDENT_FIELD_NAMES and class names
		std::list<StudentInfo> find_students(const std::string& str) const;

		//silently ignores ids that are not in the database, worst case returns empty list
		std::list<StudentInfo> get_students_by_ids(const std::set<uint>& ids) const;
		//std::list<StudentInfo> get_students_by_class(const std::string& str) const;

		//with update/add just pass in Info class it will know what to do
		template <class Info>
		bool update(const Info& info);
		template <class Info>
		bool add(Info& info);
		template <class Info>

		//with remove and get_by_id you have to explicitly select what template to use
		bool remove(uint id);
		template <class Info>
		Info get_by_id(uint id) const;
		
	private:

		std::list<unsigned> find_students_helper(const std::string& str) const;
		unsigned id_from_lookup_elem(const std::string& str) const;
		//templated getter for table pointers
		template <class Info>
		std::map<uint, std::unique_ptr<Info>>* get_ptr() const;

		//templated getter for db pointers
		template <class Info>
		BaseDao<Info>* get_dao_ptr() const;




		
		
	};




}

#endif
