#ifndef _CSVDB_H_
#define _CSVDB_H_

#include <array>
#include <string>
#include <list>
#include <memory>
#include <fstream>
#include <cstdio>

namespace cheshire
{
	template<int T_SIZE>
	class CSVDb
	{
		typedef unsigned int uint;

		const char* m_file_name;
	public:
		constexpr explicit CSVDb(const char* filename) : m_file_name(filename){}
		bool add_row(const std::array<std::string, T_SIZE> data) const;
		bool remove_row(uint id) const;
		bool update_row(const std::array<std::string, T_SIZE>& data) const;
		std::unique_ptr<std::list<std::unique_ptr<std::array<std::string, T_SIZE>>>> get_rows() const;
		std::array<std::string, T_SIZE> get_row(uint id) const;

	};
}
#endif // !_CSVDB_H_