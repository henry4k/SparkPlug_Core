#ifndef __SPARKPLUG_AABB__
#define __SPARKPLUG_AABB__

#include <SparkPlug/Vector.h>


namespace SparkPlug
{

/**
	Defines an axis aligned bounding box, e.g. a rectangle or volume.
	
	@tparam TVector vector type for min and max
**/
template< class TVector >
class aabb
{
	public:
		typedef aabb<TVector> Type;
		TVector min, max;

		aabb() {}
		aabb( const Type& v ) : min(v.min), max(v.max) {}
		aabb( const TVector& minV, const TVector& maxV ) : min(minV), max(maxV) {}

		bool operator== ( const Type& v ) const { return ((min == v.min) && (max == v.max)); }
		bool operator!= ( const Type& v ) const { return !(*this == v); }

		template< class TVector1 >
		Type& operator= ( const aabb<TVector1>& v ) { min = v.min; max = v.max; return *this; }

		operator Type() const { return Type(min, max); }
		
		TVector size() const { return max-min; }
		
		/// Restricts the volume to a range.
		void clip( const Type& bounds )
		{
			min = min.boundBy(bounds.min, bounds.max);
			max = max.boundBy(bounds.min, bounds.max);
		}
		
		/// Ensures that this volume includes another
		void include( const Type& rect )
		{
			min = min.min(rect.min);
			max = max.max(rect.max);
		}
		
		/// Moves the aabb without changing its size
		void move( const TVector& amount )
		{
			min += amount;
			max += amount;
		}
		
		
		
		
		Type operator+( const TVector& amount ) const
		{
			Type r = *this;
			r.move(amount);
			return r;
		}
		
		Type operator-( const TVector& amount ) const
		{
			Type r = *this;
			r.move(-amount);
			return r;
		}
		
		Type& operator+=( const TVector& amount )
		{
			move(amount);
			return *this;
		}
		
		Type& operator-=( const TVector& amount )
		{
			move(-amount);
			return *this;
		}
};


typedef aabb<vec2i> aabb2i;
typedef aabb<vec3i> aabb3i;

typedef aabb<vec2f> aabb2f;
typedef aabb<vec3f> aabb3f;

typedef aabb<vec2d> aabb2d;
typedef aabb<vec3d> aabb3d;

}

#endif