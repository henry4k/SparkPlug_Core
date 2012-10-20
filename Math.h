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

inline bool HighestBitValue( const uint x )
{
	assert(x != 0);
	uint bit = uint(-1);
	for(uint tmp = x; tmp; tmp >>= 1, ++bit)
		;
	return bit;
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
