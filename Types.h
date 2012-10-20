#ifndef __SPARKPLUG_TYPES__
#define __SPARKPLUG_TYPES__

#include <stdint.h>
#include <cfloat>

namespace SparkPlug
{


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef int8_t Int8;
typedef uint8_t UInt8;
typedef int16_t Int16;
typedef uint16_t UInt16;
typedef int32_t Int32;
typedef uint32_t UInt32;
typedef int64_t Int64;
typedef uint64_t UInt64;




template< class T >
class TypeInfo
{
	public:
		typedef T Float;
		typedef T Big;
};

template<>
class TypeInfo<UInt8>
{
	public:
		typedef float Float;
		typedef UInt64 Big;
		
// 		static const UInt8 Min = 0;
// 		static const UInt8 Max = 255;°
};

template<>
class TypeInfo<Int8>
{
	public:
		typedef float Float;
		typedef Int64 Big;
		
// 		static const Int8 Min = -128;
// 		static const Int8 Max = 127;
};

template<>
class TypeInfo<UInt16>
{
	public:
		typedef float Float;
		typedef UInt64 Big;
		
// 		static const UInt16 Min = 0;
// 		static const UInt16 Max = 65535;
};

template<>
class TypeInfo<Int16>
{
	public:
		typedef float Float;
		typedef Int64 Big;
		
// 		static const UInt16 Min = -32767-1;
// 		static const UInt16 Max = 32767;
};

template<>
class TypeInfo<UInt32>
{
	public:
		typedef float Float;
		typedef UInt64 Big;
		
// 		static const UInt32 Min = 0;
// 		static const UInt32 Max = 4294967295U;
};

template<>
class TypeInfo<Int32>
{
	public:
		typedef float Float;
		typedef Int64 Big;
		
// 		static const Int32 Min = -2147483647-1;
// 		static const Int32 Max = 2147483647;
};

template<>
class TypeInfo<UInt64>
{
	public:
		typedef float Float;
		typedef UInt64 Big;
		
// 		static const UInt64 Min = 0;
// 		static const UInt64 Max = 18446744073709551615ULL;
};

template<>
class TypeInfo<Int64>
{
	public:
		typedef float Float;
		typedef Int64 Big;
		
// 		static const Int64 Min = -9223372036854775807LL-1;
// 		static const Int64 Max = 9223372036854775807LL;
};

template<>
class TypeInfo<float>
{
	public:
		typedef float Float;
		typedef double Big;
		
// 		static const float Min = FLT_MIN;
// 		static const float Max = FLT_MAX;
};

template<>
class TypeInfo<double>
{
	public:
		typedef double Float;
		typedef long double Big;
		
// 		static const double Min = DBL_MIN;
// 		static const double Max = DBL_MAX;
};

template<>
class TypeInfo<long double>
{
	public:
		typedef long double Float;
		typedef long double Big;
		
// 		static const long double Min = LDBL_MIN;
// 		static const long double Max = LDBL_MAX;
};


}

#endif
