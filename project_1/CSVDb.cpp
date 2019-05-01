#include "CSVDb.h"

using namespace cheshire;

bool CSVDb::is_valid(const std::vector<std::string>& data) const
{
	for (auto& field : data)
	{
		if (field.find(',') != std::string::npos || field.find('\n') != std::string::npos) return false;
	}
	return true;
}

CSVDb::CSVDb(const std::string& filename, const std::vector<std::string>& header) : m_header(header), m_file_name(filename)
{
	// if file exists and it's not empty do nothing
	std::ifstream test(filename, std::ios::in);
	if (test.good())
	{
		std::string str;
		if (test >> str) return;
	}
	// create file if not exists, write header
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

bool CSVDb::add_row(uint id, const std::vector<std::string>& data) const
{
	if (data.empty()) return false;
	if (!is_valid(data)) return false;
	auto file = std::ofstream(m_file_name, std::ios::app);
	file << id << ',';
	auto ptr = data.begin();
	auto end = data.end();
	for (auto it : m_header)
	{

		file << ((ptr != end) ? *(ptr) : "") << ',';
		if (ptr != end) ++ptr;
	}	
	file << '\n';
	return true;
}


bool CSVDb::remove_row(unsigned id, bool remove_only_one) const
{
	bool removed = false;
	auto temp = std::ofstream("temp.txt", std::ios::out | std::ios::trunc);
	auto file = std::ifstream(m_file_name, std::ios::in);
	// read from orginal file and pipe it into temp
	// if found id don't write that row into temp
	// if row was skipped replace orignal file with temp one
	{
		std::string str;
		std::getline(file, str);
		temp << str << "\n";
		while (std::getline(file, str, ','))	
		{
			uint tempid = std::stoul(str);
			if (tempid == id && remove_only_one && !removed)
			{
				std::getline(file, str, '\n');
				removed = true;
			}
			else if (tempid == id && !remove_only_one)
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
			std::remove(m_file_name.c_str());
			std::rename("temp.txt", m_file_name.c_str());
		}
	}
	return removed;
}


bool CSVDb::update_row(uint id, const std::vector<std::string>& data) const
{
	// it always adds at the end of the file, it doesn't sort by id
	if (data.empty()) return false;
	if (!is_valid(data)) return false;
	if (remove_row(id)) return add_row(id, data);
	return false;
}


std::pair<unsigned,std::unique_ptr<std::map<const std::string, std::string>>> CSVDb::get_row(unsigned id) const
{
	// if student not found returns empty map

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


// 2 secs Faster than array version
std::unique_ptr<std::unordered_map<unsigned int, std::unique_ptr<std::map<const std::string, std::string>>>> CSVDb::get_rows(const std::set<uint>& ids) const
{
	bool get_by_ids = false;
	if (!ids.empty()) get_by_ids = true;

	auto map_ptr = std::make_unique<std::unordered_map<uint, std::unique_ptr<std::map<const std::string, std::string>>>>();
	auto file = std::ifstream(m_file_name, std::ios::in);
	
	std::string str;
	std::getline(file, str);

	while (std::getline(file, str, ','))
	{
		uint id = std::stoul(str);
		if (!get_by_ids || ids.find(id) != ids.end())
		{
			auto elem_map_ptr = std::make_unique<std::map<const std::string, std::string>>();
			for (auto& it : m_header)
			{
				std::getline(file, str, ',');
				elem_map_ptr->emplace(it, str);
				//(*elem_map_ptr)[it] = str;
			}
			map_ptr->emplace(id, std::move(elem_map_ptr));
			//(*map_ptr)[id] = std::move(elem_map_ptr);
		}
		std::getline(file, str);
	}

	return map_ptr;
}

unsigned CSVDbID::add_row(const std::vector<std::string>& data) const
{
	uint id = m_id_manager->get_id();
	if (CSVDb::add_row(id, data)) return id; else return 0;
}

bool CSVDbID::remove_row(uint id) const
{
	if (CSVDb::remove_row(id))
	{
		m_id_manager->release_id(id);
		return true;
	}
	return false;
}