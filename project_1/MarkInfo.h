#ifndef _MARKINFO_H_
#define _MARKINFO_H_
#include "BasicData.h"
#include <iostream>

namespace project
{
	const std::vector<std::string> MARK_FIELD_NAMES{"STUDENT_ID", "VALUE", "DATE", "SUBJECT", "PROFESSOR_ID"};

	enum class MARK
	{
		_2 = 4, _2_5, _3, _3_5, _4, _4_5, _5
	};
	enum class SUBJECT
	{
		MD, PE, PP
	};
	class MarkInfo : public cheshire::BasicData
	{
		using uint = unsigned;
		uint m_id = 0;
	public:

		uint m_student_id;
		uint m_professor_id;
		MARK m_value;
		std::string m_date;
		SUBJECT m_subject;

		MarkInfo() {}
		MarkInfo(uint student_id, MARK value, const std::string& date, SUBJECT subject, uint professor_id):
			m_student_id(student_id),
			m_professor_id(professor_id),
			m_value(value),
			m_date(date),
			m_subject(subject)
		{}
		//*****************INTERFACE IMPLEMENTATION*************//
		void set_id(uint id) override {m_id = id;}
		uint get_id() const override {return m_id;}
		std::vector<std::string> to_string_vector() const override
		{
			std::vector<std::string> vec(project::MARK_FIELD_NAMES.size());
			vec[0] = std::to_string(m_student_id);
			vec[1] = std::to_string(static_cast<short>(m_value));
			vec[2] = m_date;
			vec[3] = std::to_string(static_cast<short>(m_subject));
			vec[4] = std::to_string(m_professor_id);
			return vec;
		}
		void from_map(unsigned id, const std::unique_ptr<std::map<const std::string, std::string>>& data)
		{
			m_id = id;
			m_student_id = std::stoul(data->at(project::MARK_FIELD_NAMES[0]));
			m_value = project::MARK(std::stoi(data->at(project::MARK_FIELD_NAMES[1])));
			m_date = data->at(project::MARK_FIELD_NAMES[2]);
			m_subject = project::SUBJECT(std::stoi(data->at(project::MARK_FIELD_NAMES[3])));
			m_professor_id = std::stoul(data->at(project::MARK_FIELD_NAMES[4]));
		}
		//******************************************************//
		friend std::ostream& operator<<(std::ostream& out, const project::MarkInfo& mark)
		{
			out << "student_id:" << mark.m_student_id << " value: " << static_cast<float>(mark.m_value) / 2 << " subject: " << static_cast<short>(mark.m_subject);
			return out;
		}
	};
}


#endif
