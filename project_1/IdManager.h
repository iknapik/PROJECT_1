#ifndef _ID_MANAGER_H_
#define _ID_MANAGER_H_
/*
ID generator that stores used id in a file in following format:
last_unused_id, (optional)released_id1, (optional)released_id2, ...
it may not be stable with huge amount of released id;
start from 1 to unsigned int
*/
#include <vector>
#include <string>

namespace cheshire
{

	class IdManager
{
		typedef unsigned int uint;
		std::vector<uint> id_vector;
		std::string sfilename;
public:
	IdManager(const std::string& filename);
	uint get_id();
	bool release_id(uint id);
private:
	bool save();
};
}

#endif // !_ID_MANAGER_H_