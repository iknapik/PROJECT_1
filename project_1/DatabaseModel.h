//główna klasa do dostępu do bazy
//cała logika będzie tutaj
//obliczanie średnich, dostęp do danych
// przykładowo aby dodać studenta do bazy używamy
// student_model_instance.m_student_dao.add_student(...);
/*
#ifndef STUDENTMODEL_H_
#define STUDENTMODEL_H_

#include "StudentInfo.h"
#include "StudentDao.h"
#include "ClassInfo.h"
#include <regex>

namespace school {


class StudentModel
{
	typedef unsigned int uint;
	// to działa jak cache
	// żeby nie czytać z pliku co chwile
	const std::regex m_verify_name;
	const std::regex m_verify_PESEL;
	std::vector<uint> m_available_id;
	StudentDao* m_student_dao;
	std::unique_ptr<std::map<uint, std::unique_ptr<StudentInfo>>> m_students;
//	std::vector<std::unique_ptr<ClassInfo>> m_classes;
	// dzięki temu można zapisywać z i do pliku
	// po prostu używa sie funkcji które tam są
	
	
public:
	StudentModel(StudentDao* sdao) : m_student_dao(sdao), m_students(), m_verify_name("^[A-Z]\\w+\\s?[A-Z]\\w+$"), m_verify_PESEL("^\\d{11}$")
	{

	}
	bool add_student(const StudentInfo& student);
};



}

#endif
*/