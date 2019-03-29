#include "BasicData.h"
#include <sstream>
#include <set>

namespace school {
	const std::vector<std::string> CLASS_FIELD_NAMES{ "TEACHER_ID", "STUDENT_IDS" };

	class ClassInfo : public cheshire::BasicData
	{
		typedef unsigned uint;
		uint m_class_id = 0;
		uint m_teacher_id;
		std::set<uint> m_student_ids;
	public:
		explicit ClassInfo() {}
		explicit ClassInfo(uint teacher_id, const std::set<uint>& set) : m_teacher_id(teacher_id), m_student_ids(set) {}
		virtual std::vector<std::string> to_string_vector() const override
		{
			// size + 1 for teacher id
			std::vector<std::string> vec(2);
			vec[0] = std::to_string(m_teacher_id);
			auto stream = std::ostringstream();
			for (auto &i : m_student_ids)
			{
				stream << i << " ";
			}
			vec[1] = stream.str();
			return vec;
		}
		virtual void from_map(unsigned id, const std::unique_ptr<std::map<const std::string, std::string>>& map_ptr) override
		{
			m_class_id = id;
			auto stream = std::istringstream(map_ptr->at(school::CLASS_FIELD_NAMES[1]));

			std::string str;
			stream >> str;
			m_teacher_id = std::stoul(map_ptr->at(school::CLASS_FIELD_NAMES[0]));

			while (stream >> str)
			{
				m_student_ids.insert(std::stoul(str));
			}
		}
		virtual unsigned get_id() const override { return m_class_id; }

		friend std::ostream& operator<<(std::ostream& out, const ClassInfo& cla)
		{
			std::cout << "Class ID: " << cla.m_class_id << ", teacher ID: " << cla.m_teacher_id << " student ids: ";
			for (auto& i : cla.m_student_ids)
			{
				std::cout << i << " ";
			}
			return out;
		}
	};

}
