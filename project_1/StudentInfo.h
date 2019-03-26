
#ifndef STUDENTINFO_H_
#define STUDENTINFO_H_

#include <string>
#include <iostream>

namespace school
{

class StudentDao;

class StudentInfo
{
	typedef unsigned int uint;
	friend StudentDao;

public:
	std::string m_firstname;
	std::string m_lastname;
	std::string m_PESEL;
	std::string m_city;
	std::string m_address;
private:
	uint m_id;
	void set_id(uint id) { m_id = id; }


public:
	StudentInfo(const std::string& firstname, const std::string& lastname, const std::string& PESEL, const std::string& city, const std::string& address, uint id = 0) :
		m_firstname(firstname),
		m_lastname(lastname),
		m_PESEL(PESEL),
		m_id(id),
		m_city(city),
		m_address(address){}		
	uint get_id() const { return m_id; }
	friend std::ostream& operator<<(std::ostream& out, const StudentInfo& student)
	{
		std::printf("ID: %i, Imie: %s, Nazwisko: %s, PESEL: %s, Miejscowość: %s, Adres: %s",
			student.get_id(), student.m_firstname.data(), student.m_lastname.data(), student.m_PESEL.data(), student.m_city.data(), student.m_address.data());
		return out;
	}	

};




}



#endif
