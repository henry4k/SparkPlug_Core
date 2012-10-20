#ifndef __SPARKPLUG_VECTOR__
#define __SPARKPLUG_VECTOR__

#include <cmath>
#include <cstring>
#include <SparkPlug/Common.h>
#include <SparkPlug/Logic.h>
#include <SparkPlug/Types.h>

namespace SparkPlug
{


/// Container helper
template< int TDimensions, class TComponent >
class VecData
{
	public:
		TComponent m[TDimensions];
};


#define TVEC_RANGE_TEST(R) assert( (R) <= Dimensions )
#define TVEC_FOR(I) for( int I = 0; I < Dimensions; ++ I )

/// Generic vector class
/*!
	Can represent vectors of arbitary dimensions and data types.
	
	Initialization:
		Can be left uninitialized
		or initialized by a pointer to an array (must be at least hold Dimensions elements)
		or initialized by another vector (that can has a different type)
		or initialized by specifying the values directly as parameters. (e.g vec3f(1, 2, 3))
	
	Access:
		Can be accessed by index or by name.
		E.g. pos[0] or pos.x
	
	
	
	\param TDimensions Dimensionsimension count
	\param TComponent The data type
	
	\sa VecData
*/
template< int TDimensions, class TComponent >
class vec : public VecData<TDimensions,TComponent>
{
	public:
		static const int Dimensions = TDimensions;
		typedef TComponent Component;
		typedef vec<TDimensions,TComponent> Type;
		typedef VecData<TDimensions,TComponent> DataType;
		typedef typename TypeInfo<Component>::Float Float;
		
		vec() {}
		vec( const Component* V ) { assert( V ); std::memcpy(DataType::m,V,sizeof(Component)*Dimensions); }
		vec( const Type& V ) { std::memcpy(DataType::m,V.DataType::m,sizeof(Component)*Dimensions); }
		template< int Dimensions1, class Type1 > vec( const vec<Dimensions1,Type1>& V ) { for( int i = 0, max = (Dimensions1 < Dimensions)?(Dimensions1):(Dimensions); i < max; ++i ) { DataType::m[i] = V[i]; } }
// 		vec( const Component& X ) { TVEC_FOR(i) DataType::m[i] = X; }
		vec( const Component& X, const Component& Y ) { TVEC_RANGE_TEST(2); DataType::m[0] = X; DataType::m[1] = Y; }
		vec( const Component& X, const Component& Y, const Component& Z ) { TVEC_RANGE_TEST(3); DataType::m[0] = X; DataType::m[1] = Y; DataType::m[2] = Z; }
		vec( const Component& X, const Component& Y, const Component& Z, const Component& W ) { TVEC_RANGE_TEST(4); DataType::m[0] = X; DataType::m[1] = Y; DataType::m[2] = Z; DataType::m[3] = W; }

		void setAllTo( const Component& v ) { TVEC_FOR(i) DataType::m[i] = v; }

		Component& operator[] ( const int& i ) { TVEC_RANGE_TEST(i); return DataType::m[i]; }
		const Component& operator[] ( const int& i ) const { TVEC_RANGE_TEST(i); return DataType::m[i]; }

		template< int Dimensions1 >
		const vec<Dimensions1,Type>& sub( const unsigned int& offset = 0 ) const { TVEC_RANGE_TEST(Dimensions1+offset); return *reinterpret_cast< vec<Dimensions1,Type>* >( reinterpret_cast<Component*>(this)+offset ); }

		template< int Dimensions1 >
		vec<Dimensions1,Type>& sub( const unsigned int& offset = 0 ) { TVEC_RANGE_TEST(Dimensions1+offset); return *reinterpret_cast< vec<Dimensions1,Type>* >( reinterpret_cast<Component*>(this)+offset ); }

		
		/// True when the comparision is true for ALL components.
		bool operator== ( const Type& V ) const { TVEC_FOR(i) { if(V.DataType::m[i] != DataType::m[i]) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator!= ( const Type& V ) const { return !(*this == V); }
		/// True when the comparision is true for ALL components.
		bool operator> (const Type& V) const { TVEC_FOR(i) { if(DataType::m[i] <= V.DataType::m[i]) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator>= (const Type& V) const { TVEC_FOR(i) { if(DataType::m[i] < V.DataType::m[i]) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator< (const Type& V) const { TVEC_FOR(i) { if(DataType::m[i] >= V.DataType::m[i]) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator<= (const Type& V) const { TVEC_FOR(i) { if(DataType::m[i] > V.DataType::m[i]) return false; } return true; }

		/// True when the comparision is true for ALL components.
		bool operator== ( const Component& V ) const { TVEC_FOR(i) { if(V != DataType::m[i]) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator!= ( const Component& V ) const { return !(*this == V); }
		/// True when the comparision is true for ALL components.
		bool operator> (const Component& V) const { TVEC_FOR(i) { if(DataType::m[i] <= V) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator>= (const Component& V) const { TVEC_FOR(i) { if(DataType::m[i] < V) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator< (const Component& V) const { TVEC_FOR(i) { if(DataType::m[i] >= V) return false; } return true; }
		/// True when the comparision is true for ALL components.
		bool operator<= (const Component& V) const { TVEC_FOR(i) { if(DataType::m[i] > V) return false; } return true; }


		template< class TComponent1 >
		Type& operator= (const vec<Dimensions,TComponent1>& V) { TVEC_FOR(i) { DataType::m[i] = V.VecData<Dimensions,TComponent1>::m[i]; } return *this; }

		operator Type() const { return Type(DataType::m); }
		operator Component*() { return DataType::m; }
		operator const Component*() const { return DataType::m; }

// 		template<class TComponent1>
// 		operator vec<Dimensions,TComponent1>() const { TComponent1 n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i]; } return vec<Dimensions,TComponent1>(n); }



		Type operator+ (const Type& V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] + V.DataType::m[i]; } return Type(n); }
		Type operator+ (const Component& V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] + V; } return Type(n); }
		Type operator- (const Type& V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] - V.DataType::m[i]; } return Type(n); }
		Type operator- (const Component& V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] - V; } return Type(n); }
		Type operator* (const Type& V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] * V.DataType::m[i]; } return Type(n); }
		Type operator* (const Float V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = Component( Float( DataType::m[i] ) * V ); } return Type(n); }
		Type operator/ (const Type& V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] / V.DataType::m[i]; } return Type(n); }
		Type operator/ (const Float V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = Component( Float( DataType::m[i] ) / V ); } return Type(n); }
		Type operator% (const Type& V) const { Component  n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] % V.DataType::m[i]; } return Type(n); }

		Type& operator+= (const Type& V) { TVEC_FOR(i) { DataType::m[i] += V.DataType::m[i]; } return *this; }
		Type& operator+= (const Component& V) { TVEC_FOR(i) { DataType::m[i] += V; } return *this; }
		Type& operator-= (const Type& V) { TVEC_FOR(i) { DataType::m[i] -= V.DataType::m[i]; } return *this; }
		Type& operator-= (const Component& V) { TVEC_FOR(i) { DataType::m[i] -= V; } return *this; }
		Type& operator*= (const Type& V) { TVEC_FOR(i) { DataType::m[i] *= V.DataType::m[i]; } return *this; }
		Type& operator*= (const Float V) { TVEC_FOR(i) { DataType::m[i] *= V; } return *this; }
		Type& operator/= (const Type& V) { TVEC_FOR(i) { DataType::m[i] /= V.DataType::m[i]; } return *this; }
		Type& operator/= (const Float V) { TVEC_FOR(i) { DataType::m[i] /= V; } return *this; }
		Type& operator%= (const Type& V) { TVEC_FOR(i) { DataType::m[i] %= V.DataType::m[i]; } return *this; }
		
		Type operator- () const { Component n[Dimensions]; TVEC_FOR(i) { n[i] = -DataType::m[i]; } return Type(n); }


		/// Returns the smallest value for each component.
		Type min(const Type& v) const { Component n[Dimensions]; TVEC_FOR(i) { n[i] = Min(DataType::m[i], v[i]); } return Type(n); }
		/// Returns the biggest value for each component.
		Type max(const Type& v) const { Component n[Dimensions]; TVEC_FOR(i) { n[i] = Max(DataType::m[i], v[i]); } return Type(n); }
		/// Constrains each value to a range defined by a and b.
		Type boundBy( const Type& a, const Type& b ) const { Component n[Dimensions]; TVEC_FOR(i) { n[i] = BoundBy(a[i], DataType::m[i], b[i]); } return Type(n); }
		/// Returns the absolute vector.
		Type abs() const { Component n[Dimensions]; TVEC_FOR(i) { n[i] = Abs(DataType::m[i]); } return Type(n); }
		/// Typeests if one vector is completly inside the range defined by a and by.
		bool inside( const Type& a, const Type& b ) const { return (*this >= a) && (*this <= b); }

		Component sum() const { Component s = 0; TVEC_FOR(i) { s += DataType::m[i]; } return s; }

		Component lengthSqrt() const { Component l = 0; TVEC_FOR(i) { l += DataType::m[i]*DataType::m[i]; } return l; }
		/// Returns the vectors length.
		Float length() const { return std::sqrt( Float(lengthSqrt()) ); }

		/// Returns the normalized vector (length is 1)
		Type normalize() const { Float l = length(); if(!l) { return *this; } Component n[Dimensions]; TVEC_FOR(i) { n[i] = DataType::m[i] / l; } return Type(n); }

		Type floor() const { Component n[Dimensions]; TVEC_FOR(i) { n[i] = std::floor( DataType::m[i] ); } return Type(n); }
		Type ceil() const { Component n[Dimensions]; TVEC_FOR(i) { n[i] = std::ceil( DataType::m[i] ); } return Type(n); }

		/// Can be used to calculate e.g. the pixel count of an image.
		Component volume() const { Component v = DataType::m[0]; for( int i = 1; i < Dimensions; ++ i ) { v *= DataType::m[i]; } return v; }
		Component magnitudeSqrt() const { return volume(); }
		Float magnitude() const { return std::sqrt( Float( magnitudeSqrt() ) ); }
		Float magnitudeInv() const { return std::sqrt( 1.0/ Float( magnitudeSqrt() ) ); }

		/// Returns the dot product.
		Float dot(const Type& V) const { Float dp = 0; TVEC_FOR(i) { dp += DataType::m[i] * V.DataType::m[i]; } return dp; }
		
		/// Returns the cross product. (Only works for 3 dimensional vectors!!!)
		vec<3,Component> cross(const vec<3,Component>& V) const
		{
			TVEC_RANGE_TEST(3);
			return vec<3,Component>( DataType::m[1]*V.DataType::m[2] - DataType::m[2]*V.DataType::m[1], DataType::m[2]*V.DataType::m[0] - DataType::m[0]*V.DataType::m[2], DataType::m[0]*V.DataType::m[1] - DataType::m[1]*V.DataType::m[0] );
		}
		
		/// Returns the cross product. (Only works for 2 dimensional vectors!!!)
		Component cross(const vec<2,Component>& V) const
		{
			TVEC_RANGE_TEST(2);
			return DataType::m[0]*V.DataType::m[1] - DataType::m[1]*V.DataType::m[0];
		}
};


template< class TVec >
inline TVec operator+ (const typename TVec::Component& s, const TVec& v)
{
	TVec n;
	for(int i = 0; i < TVec::Dimensions; ++i)
	{
		n[i] = s + v.m[i];
	}
	return n;
}

template< class TVec >
inline TVec operator- (const typename TVec::Component& s, const TVec& v)
{
	TVec n;
	for(int i = 0; i < TVec::Dimensions; ++i)
	{
		n[i] = s - v.m[i];
	}
	return n;
}

template< class TVec >
inline TVec operator* (const typename TVec::Float s, const TVec& v)
{
	TVec n;
	for(int i = 0; i < TVec::Dimensions; ++i)
	{
		n[i] = typename TVec::Component( s * typename TVec::Float( v.m[i] ) );
	}
	return n;
}

template< class TVec >
inline TVec operator/ (const typename TVec::Float s, const TVec& v)
{
	TVec n;
	for(int i = 0; i < TVec::Dimensions; ++i)
	{
		n[i] = typename TVec::Component( s / typename TVec::Float( v.m[i] ) );
	}
	return n;
}

// #include <iostream>
// template< int TDimensions, class TComponent >
// inline std::ostream& operator<<(std::ostream& o, const vec<TDimensions,TComponent>& v ) 
// {
// 	o.precision(0);
// 	o.setf(std::ios::fixed, std::ios::floatfield);
// 	
// 	o << "Vector { ";
// 	for( int i = 0; i < TDimensions; i++ )
// 	{
// 		o << v[i] << " ";
// 	}
// 	o << "}";
// 	return o;
// }


#undef TVEC_RANGE_TEST
#undef TVEC_FOR


typedef vec<3, unsigned char> vec3ub;
typedef vec<4, unsigned char> vec4ub;

typedef vec<2, int> vec2i;
typedef vec<3, int> vec3i;
typedef vec<4, int> vec4i;

typedef vec<2, float> vec2f;
typedef vec<3, float> vec3f;
typedef vec<4, float> vec4f;

typedef vec<2, double> vec2d;
typedef vec<3, double> vec3d;
typedef vec<4, double> vec4d;


// Specializations of VecData

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu"

template< class TComponent >
class VecData<1, TComponent>
{
	public:
		union
		{
			TComponent m[1];
			struct { TComponent x; }; // Vertex
			struct { TComponent a; }; // Color
			struct { TComponent s; }; // Texture
		};
};

template< class TComponent >
class VecData<2, TComponent>
{
	public:
		union
		{
			TComponent m[2];
			struct { TComponent x, y; }; // Vertex
			struct { TComponent r, a; }; // Color
			struct { TComponent s, t; }; // Texture
			// vec<2,TComponent> xy;
		};
};

template< class TComponent >
class VecData<3, TComponent>
{
	public:
		union
		{
			TComponent m[3];
			struct { TComponent x, y, z; }; // Vertex
			struct { TComponent r, g, b; }; // Color
			struct { TComponent s, t, p; }; // Texture
			struct { TComponent yaw, pitch, roll; }; // Rotation
			// vec<2,TComponent> xy;
			// vec<3,TComponent> xyz;
		};
};

template< class TComponent >
class VecData<4, TComponent>
{
	public:
		union
		{
			TComponent m[4];
			struct { TComponent x, y, z, w; }; // Vertex
			struct { TComponent r, g, b, a; }; // Color
			struct { TComponent s, t, p, q; }; // Texture
			struct { TComponent yaw, pitch, roll; }; // Rotation
			// vec<2,TComponent> xy;
			// vec<3,TComponent> xyz;
			// vec<4,TComponent> xyzw;
		};
};

#pragma GCC diagnostic pop


}

#endif
