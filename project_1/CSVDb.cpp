#include "CSVDb.h"

using namespace cheshire;

CSVDb::CSVDb(const char* filename, const std::vector<std::string>& header) : m_header(header), m_file_name(filename)
{

	std::ifstream test(filename, std::ios::in);
	if (test.good())
	{
		std::string str;
		if (test >> str) return;
	}
	else
	{
		test.close();
		std::ofstream file(filename, std::ios::out);
		file << "ID ";
		for (auto &it : header)
		{
			file << it << " ";
		}
		file << "\n";
	}
		
}

bool CSVDb::add_row(uint id, const std::map<const std::string, std::string>& data) const
{
	if (data.empty()) return false;
	auto file = std::ofstream(m_file_name, std::ios::app);
	file << id << ',';
	for (auto it : m_header)
	{
		file << data.at(it) << ',';
	}	
	file << '\n';
	return true;
}


bool CSVDb::remove_row(unsigned id) const
{
	bool removed = false;
	auto temp = std::ofstream("temp.txt", std::ios::out | std::ios::trunc);
	auto file = std::ifstream(m_file_name, std::ios::in);
	{
		std::string str;
		std::getline(file, str);
		temp << str << "\n";
		while (std::getline(file, str, ','))	
		{
			uint tempid = std::stoul(str);
			if (tempid == id)
			{
				std::getline(file, str, '\n');
				removed = true;
			}
			else
			{
				temp << str << ',';
				std::getline(file, str, '\n');
				temp << str << '\n';
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


bool CSVDb::update_row(uint id, const std::map<const std::string, std::string>& data) const
{
	if (data.empty()) return false;
	if (remove_row(id)) return add_row(id, data);
	return false;
}


std::pair<unsigned,std::unique_ptr<std::map<const std::string, std::string>>> CSVDb::get_row(unsigned id) const
{
	// if student not found returns empty array

	auto file = std::ifstream(m_file_name, std::ios::in);
	std::string str;
	std::getline(file, str);
	auto map_ptr = std::make_unique<std::map<const std::string, std::string>>();
	while (std::getline(file, str, ','))
	{		
		uint tempid = std::stoul(str);
		if (tempid == id)
		{
			
			for (auto& it : m_header)
			{
				std::getline(file, str, ',');
				(*map_ptr)[it] = str;
			}
			return std::pair<uint,std::unique_ptr<std::map<const std::string, std::string>>>(id, std::move(map_ptr));
		}
		else
		{
			std::getline(file, str);
		}
	}

	return std::pair<uint, std::unique_ptr<std::map<const std::string, std::string>>>(id, std::move(map_ptr));
}



std::unique_ptr<std::map<unsigned int, std::unique_ptr<std::map<const std::string, std::string>>>> CSVDb::get_rows() const
{
	auto map_ptr = std::make_unique<std::map<uint, std::unique_ptr<std::map<const std::string, std::string>>>>();
	auto file = std::ifstream(m_file_name, std::ios::in);
	
	std::string str;
	std::getline(file, str);

	while (std::getline(file, str, ','))
	{
		uint id = std::stoul(str);
		auto elem_map_ptr = std::make_unique<std::map<const std::string, std::string>>();
		for (auto& it : m_header)
		{
			std::getline(file, str, ',');
			elem_map_ptr->emplace(it, str);	
		}
		map_ptr->emplace(id, std::move(elem_map_ptr));
		std::getline(file, str);
	}

	return map_ptr;
}
