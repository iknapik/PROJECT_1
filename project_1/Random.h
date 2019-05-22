#ifndef RANDOM_H_
#define RANDOM_H_
#include <random>
class Random
{
	std::mt19937 engine;
	std::uniform_int_distribution<int> random_char;
	std::uniform_int_distribution<int> random_num;
public:
	Random() : random_char(48, 126), random_num(48, 57)
	{
		std::random_device dev{};
		engine = std::mt19937(dev());
	}
	int random_int(int min, int max)
	{
		auto dist = std::uniform_int_distribution<int>(min, max);
		return dist(engine);
	}
	std::string token(short size)
	{
		std::string str(size, ' ');

		for (auto &chr : str)
		{
			chr = random_char(engine);
		}
		return str;
	}
	std::string token_rs(short min, short max)
	{
		return token(random_int(min, max));
	}
	std::string number_token(short size)
	{
		std::string str(size, ' ');
		for (auto &chr : str)
		{
			chr = random_num(engine);
		}
		return str;
	}
};

#endif