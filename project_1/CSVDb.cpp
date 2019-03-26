#include "CSVDb.h"

using namespace cheshire;

// crucial for linker to understand
template class CSVDb<5>;
template class CSVDb<3>;
template class CSVDb<6>;

template<int T_SIZE>
bool CSVDb<T_SIZE>::add_row(const std::array<std::string, T_SIZE> data) const
{
	if (data.empty()) return false;
	// check that first element is integer (id)
	std::stoul(data[0]);
	auto file = std::ofstream(m_file_name, std::ios::app);
	file << '"';
	for (auto it = data.cbegin(); it != data.cend(); ++it)
	{
		file << *it << ((it != data.cend() - 1) ? "," : "");
	}
	file << '"' << '\n';
	return true;
}

template<int T_SIZE>
bool CSVDb<T_SIZE>::remove_row(unsigned id) const
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
				uint tempid = std::stoul(str);
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

template<int T_SIZE>
bool CSVDb<T_SIZE>::update_row(const std::array<std::string, T_SIZE>& data) const
{
	unsigned id = std::stoul(data[0]);
	if (remove_row(id)) return add_row(data);
	else return false;
}

template<int T_SIZE>
std::array<std::string, T_SIZE> CSVDb<T_SIZE>::get_row(unsigned id) const
{
	// if student not found returns empty array

	auto file = std::ifstream(m_file_name, std::ios::in);
	std::array<std::string, T_SIZE> tarray;
	while (file)
	{
		if (file.get() == '"')		{
			std::string str;
			std::getline(file, str, ',');

			uint tempid = std::stoul(str);
			if (id == tempid)
			{
				tarray[0] = std::to_string(id);
				for (auto it = tarray.begin() + 1; it != tarray.end() - 1; ++it)
				{
					std::getline(file, *it, ',');
				}
				std::getline(file, *(tarray.end() - 1), '"');
				return tarray;
			}
			else
			{
				std::getline(file, str, '"');
				file.get();
			}
		}
	}
	return tarray;
}

template<int T_SIZE>
std::unique_ptr<std::list<std::unique_ptr<std::array<std::string, T_SIZE>>>> CSVDb<T_SIZE>::get_rows() const
{
	auto list_ptr = std::make_unique<std::list<std::unique_ptr<std::array<std::string, T_SIZE>>>>();
	auto file = std::ifstream(m_file_name, std::ios::in);
	while (file)
	{
		if (file.get() == '"')
		{
			auto temp_arr = std::make_unique<std::array<std::string, T_SIZE>>();
			for (auto it = temp_arr->begin(); it != temp_arr->end() - 1; ++it)
			{
				std::getline(file, *it, ',');
			}
			std::getline(file, *(temp_arr->end() - 1), '"');
			file.get();
			//std::cout << "\n" << "iter";
			list_ptr->push_back(std::move(temp_arr));
		}
		else
			return list_ptr;
	}

	return list_ptr;
}