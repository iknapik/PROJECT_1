#ifndef PROFESSORINFO_H_
#define PROFESSORINFO_H_
#include "BasicData.h"
//#include <sstream>

namespace project
{
const std::vector<std::string> PROFESSOR_FIELD_NAMES{"FIRSTNAME", "LASTNAME", "TITLE", "SUBJECTS"};

class ProfessorInfo : public cheshire::BasicData
{
	typedef unsigned uint;
	uint m_id = 0;
public:
	std::string m_first_name;
	std::string m_last_name;
	std::string m_title;
	std::vector<std::string> m_subjects;
	ProfessorInfo() {}
	ProfessorInfo(const std::string& firstname, const std::string& lastname, const std::string& title, const std::vector<std::string>& subjects):
		m_first_name(firstname),
		m_last_name(lastname),
		m_title(title),
		m_subjects(subjects)
	{}
	//***********INTERFACE IMPLEMENTATION****************//
	void set_id(uint id) override {m_id = id;}
	uint get_id() const override {return m_id;}
	std::vector<std::string> to_string_vector() const override
	{
		std::vector<std::string> vec(project::PROFESSOR_FIELD_NAMES.size());
		vec[0] = m_first_name;
		vec[1] = m_last_name;
		vec[2] = m_title;
		std::ostringstream stream;
		for(auto& i : m_subjects)
		{
			stream << i << " ";
		}
		vec[3] = stream.str();
		return vec;
	}

	void from_map(uint id, const std::unique_ptr<std::map<const std::string, std::string>>& data) override
	{
		m_id = id;
		m_first_name = data->at(project::PROFESSOR_FIELD_NAMES[0]);
		m_last_name = data->at(project::PROFESSOR_FIELD_NAMES[1]);
		m_title = data->at(project::PROFESSOR_FIELD_NAMES[2]);
		std::istringstream stream(data->at(project::PROFESSOR_FIELD_NAMES[3]));
		std::string str;
		m_subjects.clear();
		while(stream >> str)
		{
			m_subjects.push_back(str);
		}
	}
	//***************************************************//
	friend std::ostream& operator<<(std::ostream& out, const ProfessorInfo& prof)
	{
		out << prof.m_title << " " << prof.m_first_name << " " << prof.m_last_name << " subjects: ";
		for (auto &i : prof.m_subjects) out << i << ", ";
		return out;
	}
};




}
#endif /* PROFESSORINFO_H_ */
