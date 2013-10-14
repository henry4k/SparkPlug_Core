#ifndef __SPARKPLUG_INTERPOLATION__
#define __SPARKPLUG_INTERPOLATION__

#include <SparkPlug/Math.h>

namespace SparkPlug
{

template< class TValue, class TFactor = float >
class Mix
{
	public:
		typedef TValue (*MixFn)( const TValue& a, const TValue& b, const TFactor& t );
		
		/// Linear
		static TValue Linear( const TValue& a, const TValue& b, const TFactor& t )
		{
			return a + t * ( b - a );
		}
		
		/// Cosine
		static TValue Cosine( const TValue& a, const TValue& b, const TFactor& t )
		{
			return Linear(a,b,(-cos(Pi*t)/2.0+0.5));
		}
		
		/// SmoothStep
		static TValue SmoothStep( const TValue& a, const TValue& b, const TFactor& t )
		{
			return Linear(a,b,t*t*(3.0-2.0*t));
		}
		
		/// Acceleration
		static TValue Acceleration( const TValue& a, const TValue& b, const TFactor& t )
		{
			return Linear(a,b,t*t);
		}
		
		/// Deceleration
		static TValue Deceleration( const TValue& a, const TValue& b, const TFactor& t )
		{
			return Linear(a,b,1.0-(1.0-t)*(1.0-t));
		}
		
		/// Interpolates 4 values
		/*
		static TValue Mix2D( const TValue* values, const vec<2,TFactor>& factor, MixFn mixX = Linear, MixFn mixY = 0 )
		{
			if(!mixY)
				mixY = mixX;
			
			// values:
			// 0 1
			// 2 3
			
			TValue a = mixX(values[0], values[1], factor.x);
			TValue b = mixX(values[2], values[3], factor.x);
			return mixY(a, b, factor.y);
		}
		*/
};

/** http://www.iquilezles.org/www/articles/functions/functions.htm

float almostIdentity( float x, float m, float n )
{
    if( x>m ) return x;

    const float a = 2.0f*n - m
    const float b = 2.0f*m - 3.0f*n;
    const float t = x/m;

    return (a*t + b)*t*t + n;
}

static float impulse( float k, float x )
{
	const float h = k*x;
	return h*expf(1.0f-h);
}

float cubicPulse( float c, float w, float x )
{
    x = fabsf(x - c);
    if( x>w ) return 0.0f;
    x /= w;
    return 1.0f - x*x*(3.0f-2.0f*x);
}

float expPulse( float x, float k, float n )
{
    return expf( -k*powf(x,n) );
}

**/


/** http://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/

Vector3 Lerp(Vector3 start, Vector3 end, float percent)
{
     return (start + percent*(end - start));
}

Vector3 Slerp(Vector3 start, Vector3 end, float percent)
{
     // Dot product - the cosine of the angle between 2 vectors.
     float dot = Vector3.Dot(start, end);
     // Clamp it to be in the range of Acos()
     Mathf.Clamp(dot, -1.0f, 1.0f);
     // Acos(dot) returns the angle between start and end,
     // And multiplying that by percent returns the angle between
     // start and the final result.
     float theta = Mathf.Acos(dot)*percent;
     Vector3 RelativeVec = end - start*dot;
     RelativeVec.Normalize();     // Orthonormal basis
     // The final result.
     return ((start*Mathf.Cos(theta)) + (RelativeVec*Mathf.Sin(theta)));
}

Vector3 Nlerp(Vector3 start, Vector3 end, float percent)
{
     return Lerp(start,end,percent).normalized();
}

**/


}


#endif
