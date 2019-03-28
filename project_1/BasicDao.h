#ifndef BASIC_DAO_H_
#define BASIC_DAO_H_

#include "CSVDb.h"
#include "BasicData.h"

using namespace cheshire;
namespace cheshire
{

	class BasicDao
	{
		typedef unsigned int uint;
		const std::vector<std::string> m_field_names;
		const char* m_file_name;
		std::unique_ptr<cheshire::CSVDbID> m_database;
	public:
		BasicDao(const std::vector<std::string>& fieldnames, const char* filename = "db.txt") :
			m_field_names(fieldnames),
			m_file_name(filename), 
			m_database(std::make_unique<cheshire::CSVDbID>(filename, m_field_names)) {}
		bool add_data(const BasicData& data) const
		{
			return m_database->add_row(data.to_string_vector());
		}
		bool remove_data(uint id) const
		{
			return m_database->remove_row(id);
		}
		bool update_data(uint id, const BasicData& data) const
		{
			return m_database->update_row(id, data.to_string_vector());
		}
	};

}
#endif