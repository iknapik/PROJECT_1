//DATA ACCESS OBJECT
// serializacja/deserialzacja
/*
manages database // CSVDbID
it works with any type that implements BasicData
*/
#ifndef BASEDAO_H_
#define BASEDAO_H_
#include "CSVDb.h"
#include "BasicData.h"
#include <memory>
#include <map>

namespace cheshire {
	
template<class T>
class BaseDao {
	typedef unsigned int uint;

	const char* m_file_name;
	std::unique_ptr<cheshire::CSVDbID> m_database;

public:
	explicit BaseDao(const char* filename, const std::vector<std::string>& fieldnames) : m_file_name(filename), m_database(std::make_unique<cheshire::CSVDbID>(filename, fieldnames)) {}
	// redirects to CSVDb using BasicData interface
	uint add_row(const T& data) const
	{
		return m_database->add_row(data.to_string_vector());
	}
	// redirects to CSVDb using BasicData interface
	bool remove_row(uint id) const
	{
		return m_database->remove_row(id);
	}
	// redirects to CSVDb using BasicData interface
	bool update_row(const T& data) const
	{
		return m_database->update_row(data.get_id(), data.to_string_vector());
	}
	// converts from map returned by CSVDb to Map of T
	// that pointer management though!
	std::unique_ptr<std::unordered_map<uint, std::shared_ptr<T>>> get_rows(const std::set<uint>& ids = {}) const
	{
		auto out_ptr = std::make_unique<std::unordered_map<uint, std::shared_ptr<T>>>();
		auto rows_ptr = m_database->get_rows(ids);
		for (auto &map_ptr : *rows_ptr)
		{
			auto sinfo_ptr = std::make_shared<T>();
			sinfo_ptr->from_map(map_ptr.first, std::move(map_ptr.second));			
			out_ptr->emplace(map_ptr.first, std::move(sinfo_ptr));
		}

		return out_ptr;
	}

	std::unique_ptr<T> get_row(uint id) const
	{
		auto pair = m_database->get_row(id);
		auto ptr = std::make_unique<T>();
		if (!pair.second->empty()) ptr->from_map(id, std::move(pair.second));
		return ptr;
	}
};

}

#endif
