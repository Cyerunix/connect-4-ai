#pragma once
#include <time.h>
#include <cstdlib>

class Random
{
public:
	/*
	Initializes the seed by time
	*/
	static void init();

	/*
	Generates a random integer between min and max
	max must be greater than min
	@param min The minimum value to be generated
	@param max The maximum value to be generated
	@return int The random number
	*/
	static inline int nextInt(const int& min, const int& max) { return rand() % (max - min + 1) + min; }
private:
};