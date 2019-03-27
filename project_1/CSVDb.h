#ifndef _CSVDB_H_
#define _CSVDB_H_

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <cstdio>
/*
Universal CSV database class
it stores data as map<ID, DATA>
DATA itself is a map with KEYS specified in m_header
very lightweight, doesn't perfom any checks though
DONT USE: { , \n } in DATA
*/
namespace cheshire
{

	class CSVDb
	{
		typedef unsigned int uint;
		
		// for storing KEYS
		const std::vector<std::string> m_header;
		const char* m_file_name;
	public:
		explicit CSVDb(const char* filename, const std::vector<std::string>& header);
		bool add_row(uint id, const std::map<const std::string, std::string>& data) const;
		bool remove_row(uint id) const;
		bool update_row(uint id, const std::map<const std::string, std::string>& data) const;
		// returns a pair of (ID, DATA)
		std::unique_ptr<std::map<uint, std::unique_ptr<std::map<const std::string, std::string>>>> get_rows() const;
		// returns map of (ID, map of (KEYS, DATA))
		std::pair<unsigned, std::unique_ptr<std::map<const std::string, std::string>>> get_row(uint id) const;

	};
}
#endif // !_CSVDB_H_