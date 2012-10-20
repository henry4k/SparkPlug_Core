#ifndef __SPARKPLUG_RANDOM__
#define __SPARKPLUG_RANDOM__

#include <cstdlib>


namespace SparkPlug
{

void InitRandom( unsigned int seed = 0 );

template <typename T> T Random( const T& max )
{
	return T(rand()) * max / T(RAND_MAX);
}

template <typename T> T RandomX( const T& min, const T& max)
{
	if(min < max)
		return T(rand()) * (max-min) / T(RAND_MAX) + min;
	else
		return T(rand()) * (min-max) / T(RAND_MAX) + max;
}

}

#endif
