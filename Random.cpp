#include "Random.h"

void Random::init()
{
	srand(static_cast<unsigned int>(time(NULL)));
}