
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
private:
	uint m_id;
	void set_id(uint id) { m_id = id; }


public:
	StudentInfo(const std::string& firstname, const std::string& lastname, const std::string& PESEL, uint id = 0) :
		m_firstname(firstname),
		m_lastname(lastname),
		m_PESEL(PESEL),
		m_id(id){}
		
	uint get_id() const { return m_id; }
	friend std::ostream& operator<<(std::ostream& out, const StudentInfo& student)
	{
		out << '"' << student.get_id() << ',' << student.m_firstname << ',' << student.m_lastname << ',' << student.m_PESEL << '"';
		return out;
	}	

};




}



#endif
