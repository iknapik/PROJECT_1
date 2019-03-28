// classinfo ta klasa nam mówi nam jaki student należy do jakiej klasy
// i id nauczyciela prowadzącego

// classdao zajmuje się serializacją/deserializacją
// TODO//
#ifndef CLASSINFO_H_
#define CLASSINFO_H_

#include <vector>
#include <string>
#include <memory>

namespace school 
{

	class ClassInfo {
		typedef unsigned int uint;
	public:
		std::string m_name;
		std::vector<uint> m_students = {};
		uint m_teacher_id;
		ClassInfo(const std::string& name, uint teacher_id) :
			m_name(name),
			m_teacher_id(teacher_id)
		{}
	};
}
#endif
