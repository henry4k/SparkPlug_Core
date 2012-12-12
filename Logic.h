#ifndef __SPARKPLUG_LOGIC__
#define __SPARKPLUG_LOGIC__

namespace SparkPlug
{

// #define TestFlag(T,F) ( ( T & ( F )) == ( F ) )
// #define EnableFlag(T,F) T |= ( F )
// #define DisableFlag(T,F) T &= ~( F )
// #define ToggleFlag(T,F) T ^= ( F )

// ^- Als Template oder so


/// Constrains a value to a range
template<class T>
T BoundBy(const T& min, const T& v, const T& max)
{
	if(v > max)
		return max;
	if(v < min)
		return min;
	return v;
}


/// Returns the smaller value
template<class T>
T Min(const T& a, const T& b)
{
	return (a < b) ? a : b;
}


/// Returns the bigger value
template<class T>
T Max(const T& a, const T& b)
{
	return (a > b) ? a : b;
}


/// Returns the absolute value
template<class T>
T Abs(const T& v)
{
	return (v > 0) ? v : -v;
}


/// Tests if the value is inside a range
template<class T>
bool Inside(const T& min, const T& v, const T& max)
{
	return (v >= min) && (v <= max);
}


/// Tests for valid array indices
inline bool InsideArray( int index, int arraySize )
{
	return (index >= 0) && (index < arraySize);
}


/// Constrains a value to a range by wrapping it
/*!
	Can be used for example to normalize Euler angles.
	Wrap(361, 0, 360) = 1
*/
template<class T>
T Wrap(const T& min, T v, T max)
{
	max += (T)(1);
	T length = max-min;
	
	if(!length)
		return min;
	
	while(v < min)
		v += length;
	
	while(v >= max)
		v -= length;
	
	return v;
}



inline int Step( int value, int stepSize )
{
	return value + (stepSize - (value%stepSize));
}


template< int TDimensions, class TComponent > class vec;

/**
 * Access an multidimensional array
 * @tparam D how many dimensions do we have?
 * @param pos position in array
 * @param size size of the array
 */
template< int D >
int ArrayIndex( const vec<D,int>& pos, const vec<D,int>& size )
{
	int index = 0;
	for(int i = D-1; i >= 0; i--)
	{
		int j = pos[i];
		for(int k = i-1; k >= 0; k--)
		{
			j *= size[k];
		}
		index += j;
	}
	return index;
}


}

#endif
