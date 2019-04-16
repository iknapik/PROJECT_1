#ifndef BASIC_DATA_H_
#define BASIC_DATA_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
/*
if you implement this interface
your class can work with BaseDao
*/

namespace cheshire
{

class BasicData
{
public:
	virtual std::vector<std::string> to_string_vector() const = 0;
	// create your object from map produced by database
	// Keys should be same as inserted when creating database
	// create keys in info class, then use same keys when creating database
	virtual void from_map(unsigned id, const std::unique_ptr<std::map<const std::string, std::string>>&) = 0;
	virtual unsigned get_id() const = 0;
	virtual void set_id(unsigned id) = 0;
	bool operator==(const BasicData& info)
	{
		return this->to_string_vector() == info.to_string_vector();
	}
};

}
#endif
