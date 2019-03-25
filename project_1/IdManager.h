#ifndef _ID_MANAGER_H_
#define _ID_MANAGER_H_

#include <vector>

namespace school
{

	class IdManager
{
		typedef unsigned int uint;
		std::vector<uint> id_vector;
		const char* sfilename = "student_id_db.txt";
public:
	IdManager(const char* filename);
	uint get_id();
	bool release_id(uint id);
private:
	bool save();

};
}

#endif // !_ID_MANAGER_H_