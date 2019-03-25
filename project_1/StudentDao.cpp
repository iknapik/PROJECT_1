
#include "StudentDao.h"

#include <fstream>
#include <string>
#include <cstdio>

using namespace school;


bool StudentDao::add_student(const StudentInfo& student) const
{
	auto file = std::ofstream(m_file_name, std::ios::app);
	file << student << '\n';
	return true;
}

StudentInfo StudentDao::get_student(uint id) const
{
	// if student not found returns StudentInfo with id 0

	auto file = std::ifstream(m_file_name, std::ios::in);
	while (file)
	{
		if (file.get() == '"')
		{
			std::string str;

			std::getline(file, str, ',');
			//std::cout << "\n*****" + str + "******\n";
			uint tempid = std::stoi(str);
			if (id == tempid)
			{
				std::string firstname, lastname, pesel;
				std::getline(file, firstname, ',');
				std::getline(file, lastname, ',');
				std::getline(file, pesel, '"');
				auto sinfo = StudentInfo(firstname, lastname, pesel);
				sinfo.set_id(id);
				return sinfo;
			}
			else
			{
				std::getline(file, str, '"');
				file.get();
			}
		}
	}
	auto sinfo = StudentInfo("", "", "");
	sinfo.set_id(0);
	return sinfo;
}

bool StudentDao::update_student(const StudentInfo& student) const
{
	if (remove_student(student.get_id())) return add_student(student);
	return false;
}

bool StudentDao::remove_student(uint id) const
{
	bool removed = false;
	auto temp = std::ofstream("temp.txt", std::ios::out | std::ios::trunc);
	auto file = std::ifstream(m_file_name, std::ios::in);
	{
		while (file)
		{
			if (file.get() == '"')
			{
				std::string str;
				std::getline(file, str, ',');
				uint tempid = std::stoi(str);
				if (tempid == id)
				{
					std::getline(file, str, '"');
					file.get();
					removed = true;
				}
				else
				{
					temp << '"' << str << ',';
					std::getline(file, str, '\n');
					temp << str << '\n';
				}
			}
		}
		if (removed)
		{
			file.close();
			temp.close();
			//std::cout << "\nRENAMING\n";
			std::remove(m_file_name);
			std::rename("temp.txt", m_file_name);
		}
	}
	return removed;
}

template<>
struct std::less<std::unique_ptr<StudentInfo>>
{
	bool operator()(const std::unique_ptr<StudentInfo>& s1, const std::unique_ptr<StudentInfo>& s2)
	{
		return s1->get_id() < s2->get_id();
	}
};


std::unique_ptr<std::map<unsigned int, std::unique_ptr<StudentInfo>>> StudentDao::get_students() const
{
	auto map_ptr = std::make_unique<std::map<unsigned int, std::unique_ptr<StudentInfo>>>();
	auto file = std::ifstream(m_file_name, std::ios::in);
	while (file)
	{
		if (file.get() == '"')
		{
			std::string str;
			std::getline(file, str, ',');
			uint tempid = std::stoi(str);
			std::string firstname, lastname, pesel;
			std::getline(file, firstname, ',');
			std::getline(file, lastname, ',');
			std::getline(file, pesel, '"');
			file.get();
			//std::cout << "\n" << "iter";
			auto s_ptr = std::make_unique<StudentInfo>(firstname, lastname, pesel);
			s_ptr->set_id(tempid);
			(*map_ptr)[tempid] = std::move(s_ptr);
		}
		else
			return map_ptr;
	}

		return map_ptr;
}
