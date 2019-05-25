
#ifndef STUDENTINFO_H_
#define STUDENTINFO_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "BasicData.h"

//Simple lightweigh class that stores data about students
//implements BasicData iterface to work with BaseDao

namespace project
{
// make sure to pass this vector to BaseDao constructor
// or from_map will be throwing out_of_range exceptions
const std::vector<std::string> STUDENT_FIELD_NAMES{"FIRSTNAME", "LASTNAME", "PESEL", "CITY", "ADDRESS", "CLASS_ID"};


class StudentInfo : public cheshire::BasicData
{
	typedef unsigned int uint;


public:
	std::string m_firstname;
	std::string m_lastname;
	std::string m_PESEL;
	std::string m_city;
	std::string m_address;
	uint m_class_id;
private:
	uint m_id = 0;
public:
	StudentInfo() : m_firstname("FIRSTNAME"),
		m_lastname("LASTNAME"),
		m_PESEL("00000000000"),
		m_city("CITY"),
		m_address("ADDRESS"),
		m_class_id(0) {
		
	}
	StudentInfo(const std::string& firstname, const std::string& lastname, const std::string& PESEL, const std::string& city, const std::string& address, uint class_id = 0) :
		m_firstname(firstname),
		m_lastname(lastname),
		m_PESEL(PESEL),	
		m_city(city),
		m_address(address),
		m_class_id(class_id)
	{}

	
	

	bool empty() const { return m_firstname.empty(); }

	unsigned get_class_id() const { return m_class_id; }
	friend std::ostream& operator<<(std::ostream& out, const StudentInfo& student)
	{
		std::printf("ID: %i, Imie: %s, Nazwisko: %s, PESEL: %s, Miejscowość: %s, Adres: %s",
			student.get_id(), student.m_firstname.data(), student.m_lastname.data(), student.m_PESEL.data(), student.m_city.data(), student.m_address.data());
		return out;
	}	


	//****************** <INTERFACE IMPLEMENTATION> ******************//
	void set_id(uint id) override { m_id = id; }
	std::vector<std::string> to_string_vector() const override
	{
		std::vector<std::string> vec(6);
		vec[0] = m_firstname;
		vec[1] = m_lastname;
		vec[2] = m_PESEL;
		vec[3] = m_city;
		vec[4] = m_address;
		vec[5] = std::to_string(m_class_id);
			return vec;
	};
	void from_map(unsigned id, const std::unique_ptr<std::map<const std::string, std::string>>& map) override
	{
		if (map->empty()) return;

		m_id = id;
		m_firstname = map->at(project::STUDENT_FIELD_NAMES[0]);
		m_lastname = map->at(project::STUDENT_FIELD_NAMES[1]);
		m_PESEL = map->at(project::STUDENT_FIELD_NAMES[2]);
		m_city = map->at(project::STUDENT_FIELD_NAMES[3]);
		m_address = map->at(project::STUDENT_FIELD_NAMES[4]);
		m_class_id = std::stoul(map->at(project::STUDENT_FIELD_NAMES[5]));
	}
	unsigned get_id() const override { return m_id; }	
	
	//****************** </INTERFACE IMPLEMENTATION> ******************//
};




}



#endif
