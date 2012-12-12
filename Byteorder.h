#ifndef __SPARKPLUG_BYTEORDER__
#define __SPARKPLUG_BYTEORDER__


#include <SparkPlug/Types.h>


namespace SparkPlug
{

/// true if the this is an little endian system
extern bool IsLittleEndian;



inline ushort EndianSwap16( ushort n )
{
	return (n<<8) | (n>>8);
}

inline uint EndianSwap32( uint n )
{
	return (n<<24) | (n>>24) | ((n>>8)&0xFF00) | ((n<<8)&0xFF0000);
}

template<class T>
inline T EndianSwap( T n )
{
	union { T t; uint i; } conv;
	conv.t = n;
	conv.i = EndianSwap32(conv.i);
	return conv.t;
}

template<> inline ushort EndianSwap<ushort>( ushort n ) { return EndianSwap16(n); }
template<> inline short EndianSwap<short>( short n ) { return EndianSwap16(n); }
template<> inline uint EndianSwap<uint>( uint n ) { return EndianSwap32(n); }
template<> inline int EndianSwap<int>(int n) { return EndianSwap32(n); }

template<class T> inline void EndianSwap( T* data, int length )
{
	for(T* end = &data[length]; data < end; data++)
		*data = EndianSwap(*data);
}


/**
	Converts a value to little endian
	@tparam T value type
	@param n the value
	@return the converted value
**/
template<class T> inline T LittleEndian( T n ) { return IsLittleEndian ? n : EndianSwap(n); }

/**
	Converts an array to little endian
	@tparam T value type
	@param buf the array
	@param len the array length
**/
template<class T> inline void LittleEndian( T* data, int length ) { if(!IsLittleEndian) EndianSwap(data, length); }

/**
	Converts a value to big endian
	@tparam T value type
	@param n the value
	@return the converted value
**/
template<class T> inline T BigEndian( T n ) { return IsLittleEndian ? EndianSwap(n) : n; }

/**
	Converts an array to big endian
	@tparam T value type
	@param buf the array
	@param len the array length
**/
template<class T> inline void BigEndian( T* data, int length ) { if(IsLittleEndian) EndianSwap(data, length); }


}

#endif
