#ifndef _CSVDB_H_
#define _CSVDB_H_

#include <map>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <fstream>
#include <cstdio>

namespace cheshire
{

	class CSVDb
	{
		typedef unsigned int uint;
		const std::vector<std::string> m_header;
		const char* m_file_name;
	public:
		explicit CSVDb(const char* filename, const std::vector<std::string>& header);
		bool add_row(uint id, const std::map<const std::string, std::string>& data) const;
		bool remove_row(uint id) const;
		bool update_row(uint id, const std::map<const std::string, std::string>& data) const;
		std::unique_ptr<std::map<uint, std::unique_ptr<std::map<const std::string, std::string>>>> get_rows() const;
		std::pair<unsigned, std::unique_ptr<std::map<const std::string, std::string>>> get_row(uint id) const;

	};
}
#endif // !_CSVDB_H_