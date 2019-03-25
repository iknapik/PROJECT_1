#ifndef _MARKINFO_H_
#define _MARKINFO_H_
#include <map>
#include <vector>
#include <string>

namespace school
{
	enum class VALUE
	{
		_1, _1_5, _2, _2_5, _3, _3_5, _4, _4_5, _5, _5_5, _6
	};
	struct Mark
	{
		VALUE value;
		short day;
		short month;
		int year;
		unsigned int teacher_id;
	};
	class MarkInfo
	{
		typedef unsigned int uint;

		static std::vector<std::string> m_allowed_subject_names;
		uint m_student_id;
	public:
		std::map<std::string, std::vector<Mark>> m_marks;
	};

	std::vector<std::string> MarkInfo::m_allowed_subject_names = { "angielski", "polski", "informatyka", "metodyka studiowania" };
}



#endif