#include "IdManager.h"
#include <fstream>
#include <sstream>

using namespace cheshire;

IdManager::IdManager(const std::string& filename)
{
	sfilename = filename;
	auto file = std::ifstream(sfilename, std::ios::in | std::ios::binary);
	while (true)
	{

		char buffer[4];
		file.read(buffer, 4);
		if (!file.good()) break;
		unsigned id{*(reinterpret_cast<unsigned*>(&buffer))};
		id_vector.push_back(id);
	}

	if (id_vector.empty()) id_vector.push_back(1);
}

unsigned int IdManager::get_id()
{
	if (id_vector.size() == 1)
	{
		uint id = id_vector[0]++;
		save();
		return id;
	}
	else
	{
		uint id = id_vector[1];
		id_vector.erase(id_vector.begin() + 1);
		save();
		return id;
	}
}

bool IdManager::save()
{
	auto file = std::ofstream(sfilename, std::ios::trunc | std::ios::out | std::ios::binary);
	for (auto id : id_vector)
	{
		char* buffer{ reinterpret_cast<char*>(&id) };
		file.write(buffer, 4);
	}
	return true;
}


bool IdManager::release_id(unsigned int id)
{
	// checks if id was used
	if (id < id_vector[0])
	{
		// checks if id wasn't already released
		for (auto i : id_vector) if (i == id) return false;
		id_vector.push_back(id);
		save();
		return true;
	}
	else
		return false;
}