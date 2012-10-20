#include <ctime>
#include <SparkPlug/Random.h"

namespace SparkPlug
{

void InitRandom( unsigned int seed )
{
	if(!seed)
		seed = (unsigned int)time(0);

	srand(seed);
}

}
