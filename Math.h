#ifndef __SPARKPLUG_MATH__
#define __SPARKPLUG_MATH__

// Math, not even once ..

#include <cmath>
#include <cfloat>
#include <SparkPlug/Common.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace SparkPlug
{

const float Pi = M_PI;

inline float Deg2Rad( float deg ) { return deg*Pi/180.f; }
inline float Rad2Deg( float rad ) { return rad*180.f/Pi; }

inline uint HighestBit( const uint value )
{
	assert(value != 0);
	uint bit = -1;
	for(uint tmp = value; tmp; tmp >>= 1, ++bit)
		;
	return bit;
}

inline uint HighestBitValue( const uint value )
{
	uint tmp = value;
	uint result = 0;
	while(tmp)
	{
		result = (tmp & (~tmp + 1)); // grab lowest bit
		tmp &= ~result; // clear lowest bit
	}
	return result;
}

inline bool IsPowerOfTwo( const uint x )
{
	return ((x != 0) && ((x & (~x + 1)) == x));
}

inline uint PowerOfTwoAbove( const uint x )
{
	return IsPowerOfTwo(x) ? x : HighestBitValue(x) << 1;
}

inline uint PowerOfTwoBelow( const uint x )
{
	return IsPowerOfTwo(x) ? x : HighestBitValue(x);
}

}

#endif
