#ifndef CLASS_INFO_H_
#define CLASS_INFO_H_


#include "BasicData.h"
#include <sstream>
#include <set>
#include <iostream>

namespace project {
	const std::vector<std::string> CLASS_FIELD_NAMES{ "CLASS_NAME", "SEMESTER" };

	class ClassInfo : public cheshire::BasicData
	{
		typedef unsigned uint;
		uint m_class_id = 0;
	public:
		std::string m_name;		
		short m_semester;	
	
		ClassInfo() {}
		ClassInfo(const std::string& class_name, short semester) : m_name(class_name), m_semester(semester) {}

		void set_id(uint id) override { m_class_id = id; }
		std::vector<std::string> to_string_vector() const override
		{
			// size + 1 for teacher id
			std::vector<std::string> vec(2);
			vec[0] = m_name;			
			vec[1] = std::to_string(m_semester);
			return vec;
		}
		void from_map(unsigned id, const std::unique_ptr<std::map<const std::string, std::string>>& map_ptr) override
		{
			m_class_id = id;
			m_name = map_ptr->at(CLASS_FIELD_NAMES[0]);
			m_semester = std::stoi(map_ptr->at(CLASS_FIELD_NAMES[1]));
			
		}
		unsigned get_id() const override { return m_class_id; }

		friend std::ostream& operator<<(std::ostream& out, const ClassInfo& cla)
		{
			std::cout << "Class ID: " << cla.m_class_id << ", name: " << cla.m_name << " semester: " << cla.m_semester;			
			return out;
		}
	};

}
#endif // CLASS_INFO_H_