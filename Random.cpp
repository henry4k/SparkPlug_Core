#include <ctime>
#include <SparkPlug/Random.h>

namespace SparkPlug
{

void InitRandom( unsigned int seed )
{
	if(!seed)
		seed = (unsigned int)std::time(0);

	std::srand(seed);
}

}
