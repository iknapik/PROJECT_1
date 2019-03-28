
#ifndef STUDENTINFO_H_
#define STUDENTINFO_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "BasicData.h"

namespace school
{
	//using namespace cheshire;
const std::vector<std::string> FIELD_NAMES{"FIRSTNAME", "LASTNAME", "PESEL", "CITY", "ADDRESS"};
class StudentDao;

class StudentInfo : public cheshire::BasicData
{
	typedef unsigned int uint;
	//friend StudentDao;

public:
	std::string m_firstname;
	std::string m_lastname;
	std::string m_PESEL;
	std::string m_city;
	std::string m_address;
private:
	uint m_id;
public:
	StudentInfo() {}
	StudentInfo(const std::string& firstname, const std::string& lastname, const std::string& PESEL, const std::string& city, const std::string& address, uint id = 0) :
		m_firstname(firstname),
		m_lastname(lastname),
		m_PESEL(PESEL),
		m_id(id),
		m_city(city),
		m_address(address){}		
	//uint get_id() const { return m_id; }
	friend std::ostream& operator<<(std::ostream& out, const StudentInfo& student)
	{
		std::printf("ID: %i, Imie: %s, Nazwisko: %s, PESEL: %s, Miejscowość: %s, Adres: %s",
			student.get_id(), student.m_firstname.data(), student.m_lastname.data(), student.m_PESEL.data(), student.m_city.data(), student.m_address.data());
		return out;
	}	

	std::vector<std::string> to_string_vector() const override
	{
		std::vector<std::string> vec(5);
		vec[0] = m_firstname;
		vec[1] = m_lastname;
		vec[2] = m_PESEL;
		vec[3] = m_city;
		vec[4] = m_address;
			return vec;
	};
	void from_map(unsigned id, const std::unique_ptr<std::map<const std::string, std::string>>& map) override
	{
		m_id = id;
		m_firstname = map->at(school::FIELD_NAMES[0]);
		m_lastname = map->at(school::FIELD_NAMES[1]);
		m_PESEL = map->at(school::FIELD_NAMES[2]);
		m_city = map->at(school::FIELD_NAMES[3]);
		m_address = map->at(school::FIELD_NAMES[4]);
	}
	unsigned get_id() const override { return m_id; }
	bool empty() const { return m_firstname.empty(); }

};




}



#endif
