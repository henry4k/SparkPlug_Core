#ifndef __SPARKPLUG_QUATERNION__
#define __SPARKPLUG_QUATERNION__

#include <cmath>
#include <cstring>
#include <SparkPlug/Vector.h>


namespace SparkPlug
{

class Quaternion
{
	public:
		float x, y, z, w;
		
		Quaternion() {}
		Quaternion( const vec4f& xyzw ) : x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w) {}
		Quaternion( const float& x_, const float& y_, const float& z_, const float& w_ ) : x(x_), y(y_), z(z_), w(w_) {}
		Quaternion( const vec3f& axis, const float& angle ) { setRotation(axis, angle); }
		Quaternion( const vec3f& ypr ) { setEuler(ypr); }
		
		void setRotation( const vec3f& axis, const float& angle)
		{
			float d = axis.length();
			float s = std::sin(angle * 0.5f) / d;

			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s; 
			w = std::cos(angle * 0.5f);
		}
		
		void setEuler( const vec3f& v )
		{
			float halfYaw = v.yaw * 0.5f;
			float halfPitch = v.pitch * 0.5f;
			float halfRoll = v.roll * 0.5f;
			float cosYaw = std::cos(halfYaw);
			float sinYaw = std::sin(halfYaw);
			float cosPitch = std::cos(halfPitch);
			float sinPitch = std::sin(halfPitch);
			float cosRoll = std::cos(halfRoll);
			float sinRoll = std::sin(halfRoll);
			
			x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
			y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
			z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
			w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		}
		
		Quaternion& operator+=( const Quaternion& q )
		{
			x += q.x;
			y += q.y;
			z += q.z;
			w += q.w;
			return *this;
		}

		Quaternion operator+( const Quaternion& q )
		{
			return Quaternion(x+q.x, y+q.y, z+q.z, w+q.w);
		}

		Quaternion& operator-=( const Quaternion& q )
		{
			x -= q.x;
			y -= q.y;
			z -= q.z;
			w -= q.w;
			return *this;
		}
		
		Quaternion operator-( const Quaternion& q )
		{
			return Quaternion(x-q.x, y-q.y, z-q.z, w-q.w);
		}
		
		Quaternion& operator*=( const Quaternion& q )
		{
			Quaternion r = *this;
			x = r.w*q.x + r.x*q.w + r.y*q.z - r.z*q.y;
			y = r.w*q.y + r.y*q.w + r.z*q.x - r.x*q.z;
			z = r.w*q.z + r.z*q.w + r.x*q.y - r.y*q.x;
			w = r.w*q.w - r.x*q.x - r.y*q.y - r.z*q.z;
			return *this;
		}
		
		Quaternion operator*( const Quaternion& q ) const
		{
			return Quaternion(
				w*q.x + x*q.w + y*q.z - z*q.y,
				w*q.y + y*q.w + z*q.x - x*q.z,
				w*q.z + z*q.w + x*q.y - y*q.x,
				w*q.w - x*q.x - y*q.y - z*q.z
			);
		}
		
		Quaternion& operator*=( const float& s )
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}
		
		Quaternion operator*( const float& s ) const
		{
			return Quaternion(x*s, y*s, z*s, w*s);
		}
		
		vec3f operator*( const vec3f& v ) const
		{
			vec3f r;
			float a00 = w * w;
			float a01 = w * x;
			float a02 = w * y;
			float a03 = w * z;
			float a11 = x * x;
			float a12 = x * y;
			float a13 = x * z;
			float a22 = y * y;
			float a23 = y * z;
			float a33 = z * z;
			r.x = v.x * (+a00 + a11 - a22 - a33) + 2 * (a12 * v.y + a13 * v.z + a02 * v.z - a03 * v.y);
			r.y = v.y * (+a00 - a11 + a22 - a33) + 2 * (a12 * v.x + a23 * v.z + a03 * v.x - a01 * v.z);
			r.z = v.z * (+a00 - a11 - a22 + a33) + 2 * (a13 * v.x + a23 * v.y - a02 * v.x + a01 * v.y);
			return r;
		}
			
	
		Quaternion& operator/=( const float& s )
		{
			x /= s;
			y /= s;
			z /= s;
			w /= s;
			return *this;
		}
		
		Quaternion operator/( const float& s )
		{
			return Quaternion(x/s, y/s, z/s, w/s);
		}
		
		float dot( const Quaternion& q ) const
		{
			return x*q.x + y*q.y + z*q.z + w*q.w;
		}
		
		float lengthSqrt() const
		{
			return dot(*this);
		}
		
		float length() const
		{
			return std::sqrt(lengthSqrt());
		}
		
		Quaternion& normalize()
		{
			return *this /= length();
		}
		
		Quaternion normalized()
		{
			return *this / length();
		}
		
		/// Return the angle between this quaternion and the other 
		float angle( const Quaternion& q ) const 
		{
			float s = std::sqrt(lengthSqrt() * q.lengthSqrt());
			return std::acos(dot(q) / s);
		}
		
		/// Return the angle of rotation represented by this quaternion
		float angle() const 
		{
			return 2.0f * std::acos(w);
		}
		
		/// Return the axis of the rotation represented by this quaternion
		vec3f axis() const
		{
			float s_squared = 1.0f - std::pow(w, 2.0f);
// 			if(s_squared < 10.f * FLT_EPSILON) //Check for divide by zero
// 				return vec3f(1.0, 0.0, 0.0);  // Arbitrary
			float s = std::sqrt(s_squared);
			return vec3f(x/s, y/s, z/s);
		}
		
		Quaternion inverse() const
		{
			return Quaternion(-x, -y, -z, w);
		}
		
		Quaternion operator-() const
		{
			return inverse();
		}
		
		Quaternion slerp( const Quaternion& q, const float& t ) const
		{
			float theta = angle(q);
			if (theta != 0.0f)
			{
				float d = 1.0f / std::sin(theta);
				float s0 = std::sin((1.0f - t) * theta);
				float s1 = std::sin(t * theta);   
				if(dot(q) < 0) // Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
				return Quaternion((x * s0 + -q.x * s1) * d,
						(y * s0 + -q.y * s1) * d,
						(z * s0 + -q.z * s1) * d,
						(w * s0 + -q.w * s1) * d);
				else
				return Quaternion((x * s0 + q.x * s1) * d,
						(y * s0 + q.y * s1) * d,
						(z * s0 + q.z * s1) * d,
						(w * s0 + q.w * s1) * d);
				
			}
			else
			{
				return *this;
			}
		}
		
		void setIdentity()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 1;
		}
		
		static Quaternion _GenIdentity()
		{
			Quaternion q;
			q.setIdentity();
			return q;
		}
		
		static const Quaternion& Identity()
		{
			static Quaternion q = _GenIdentity();
			return q;
		}
};


// #include <iostream>
// inline std::ostream& operator<<(std::ostream& o, const Quaternion& q ) 
// {
// 	o.precision(0);
// 	o.setf(std::ios::fixed, std::ios::floatfield); 
// 	o << "Quaternion { ";
// 	o << "x:" << q.x << " ";
// 	o << "y:" << q.y << " ";
// 	o << "z:" << q.z << " ";
// 	o << "w:" << q.w << " }";
// 	return o;
// }


}


#endif
