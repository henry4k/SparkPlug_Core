#ifndef __SPARKPLUG_MATRIX__
#define __SPARKPLUG_MATRIX__

#include <cmath>
#include <cstring>
#include <cassert>
#include <SparkPlug/Logic.h"
#include <SparkPlug/Vector.h"
#include <SparkPlug/Quaternion.h"

namespace SparkPlug
{

#define TMAT mat<X,Y,T>
#define TMAT_RANGETEST(A) assert(A <= X)
// #define TMAT_RANGETEST(A,B) assert( (A <= X) && (B <= Y) )

// |------------- x
// |  0  4  8 12
// |  1  5  8 13
// |  2  6 10 14
// |  3  7 11 15
// y

template< int X, int Y, class T >
class mat
{
	public:
		T m[X*Y];

		mat() {}
		mat( const T* V ) { assert( V ); std::memcpy(m, V, sizeof(T)*X*Y); }
		mat( const TMAT& V ) { std::memcpy(m, V.m, sizeof(T)*X*Y); }
		explicit mat( const Quaternion& q ) { setRotation(q); }

		/// Copy construct from another arbitary matrix
		template<int A, int B, class C> mat( const mat<A,B,C>& V )
		{
			int maxX = Min(A,X);
			int maxY = Min(B,Y);
			for( int y = 0; y < maxY; ++y )
				for( int x = 0; x < maxX; ++x )
					at(x,y) = V.at(x,y);
		}


// 		T* operator[] ( const int& y ) { TMAT_RANGETEST(y); return &m[y*X]; }
// 		const T* operator[] ( const int& y ) const { TMAT_RANGETEST(y); return &m[y*X]; }
		// ^- Don't use this, it confuses people because x and y are swapped (y comes first)!

		T& at( const int& x, const int& y ) { return m[x*Y+y]; }
		const T& at( const int& x, const int& y ) const { return m[x*Y+y]; } // y*X+x

		vec<X,T> row( const int& y ) const
		{
			vec<X,T> r;
			for(int i = 0; i < X; i++)
				r.m[i] = at(i,y);
			return r;
		}

		vec<X,T>& column( const int& x ) { return reinterpret_cast< vec<X,T>& >( m[x*Y] ); }
		const vec<X,T>& column( const int& x ) const { return reinterpret_cast< vec<X,T>& >( m[x*Y] ); }

		bool operator== ( const TMAT& V ) const { for(int i = 0; i < X*Y; i++) { if(V.m[i] != m[i]) return false; } return true; }
		bool operator!= ( const TMAT& V ) const { return !(*this == V); }


		operator TMAT() const { return TMAT(m); }
		operator T*() { return m; }
		operator const T*() const { return m; }


		void setIdentity()
		{
			for( int x = 0; x < X; x++ )
				for( int y = 0; y < Y; y++ )
					at(x,y) = (x == y)?(1):(0);
		}

		static const TMAT _GenIdentity()
		{
			TMAT v;
			v.setIdentity();
			return v;
		}

		static const TMAT& Identity()
		{
			static TMAT v = _GenIdentity();
			return v;
		}


		T tdot( const vec<Y,T>& v, const int& j ) const
		{
			T r = at(j,0)*v[0];
			for(int i = 1; i < X; i++)
				r += at(j,i)*v[i];
			return r;
		}

		vec<Y,T> transform( const vec<Y,T>& v ) const
		{
			vec<Y,T> r;

			for( int y = 0; y < Y; ++y )
			{
				T& t = r[y];
				t = 0;

				for( int x = 0; x < X; ++x )
				{
					t += at(x,y) * v[x];
				}
			}

			return r;
		}

		static TMAT GenScaleMatrix( const vec<Y,T>& v )
		{
			TMAT r = Identity();
			for(int i = 0; i < Y; i++)
			{
				r.at(i,i) = v[i];
			}
			return r;
		}

		/*
		TMAT& operator*= ( const vec<Y,T>& v )
		{
			TMAT n;
			for(int y = 0; y < Y; y++)
			{
				for(int x = 0; x < X; x++)
				{
					T& t = n.at(x,y);
					t = 0;

					for( int i = 0; i < X; i++ )
						t += at(y,i) * v[i];
				}
			}
			*this = n;
			return *this;
		}
		*/

		TMAT& translate( const vec<Y,T>& v )
		{
			TMAT n;
			for(int x = 0; x < X; x++)
			{
				if(x == X-1)
				{
					for(int y = 0; y < Y; y++)
						n.at(x,y) = v[y];
				}
				else
				{
					for(int y = 0; y < Y; y++)
						n.at(x,y) = (x == y)?(1):(0);
				}
			}

			*this *= n;
			return *this;
		}

		TMAT& operator*= ( const TMAT& v )
		{
			TMAT n;

			for( int x = 0; x < X; ++x )
			{
				for( int y = 0; y < Y; ++y )
				{
					T& t = n.at(x,y);
					t = 0;

					// A(x, 0->y) * B(0->x, y)
					for( int i = 0; i < X; i++ )
						t += at(i,y) * v.at(x,i);
				}
			}

			*this = n;
			return *this;
		}

		TMAT& operator*= ( const T& v )
		{
			for( int i = 0; i < X*Y; i++ )
				m[i] *= v;
			return *this;
		}

		TMAT operator* ( const T& v ) const
		{
			TMAT r;
			for( int i = 0; i < X*Y; i++ )
				r[i] = m[i] * v;
			return r;
		}

		TMAT& operator+= ( const TMAT& v )
		{
			for( int i = 0; i < X*Y; i++ )
				m[i] += v[i];
			return *this;
		}

		TMAT operator+ ( const TMAT& v ) const
		{
			TMAT r;
			for( int i = 0; i < X*Y; i++ )
				r[i] = m[i] + v[i];
			return *this;
		}

		TMAT& operator-= ( const TMAT& v )
		{
			for( int i = 0; i < X*Y; i++ )
				m[i] += v[i];
			return *this;
		}


		void setRotation( const Quaternion& q )
		{
			assert((X >= 3) || (Y >= 3));

			setIdentity();

			float d = q.lengthSqrt();
			assert(d != 0.0f);
			float s = 2.0f / d;

			float xs = q.x * s,   ys = q.y * s,   zs = q.z * s;
			float wx = q.w * xs,  wy = q.w * ys,  wz = q.w * zs;
			float xx = q.x * xs,  xy = q.x * ys,  xz = q.x * zs;
			float yy = q.y * ys,  yz = q.y * zs,  zz = q.z * zs;

			at(0,0) = 1.0f - (yy + zz);
			at(0,1) = xy - wz;
			at(0,2) = xz + wy;

			at(1,0) = xy + wz;
			at(1,1) = 1.0f - (xx + zz);
			at(1,2) = yz - wx;

			at(2,0) = xz - wy;
			at(2,1) = yz + wx;
			at(2,2) = 1.0f - (xx + yy);
		}

		Quaternion getRotation() const
		{
			float trace = at(0,0) + at(1,1) + at(22);
			vec4f temp;

			if(trace > 0.0)
			{
				float s = std::sqrt(trace + 1.0f);
				temp.m[3] = (s * 0.5f);
				s = 0.5f / s;

				temp.m[0] = (at(1,2) - at(2,1)) * s;
				temp.m[1] = (at(2,0) - at(0,2)) * s;
				temp.m[2] = (at(0,1) - at(1,0)) * s;
			}
			else
			{
				int i = at(0,0) < at(1,1) ?
					(at(1,1) < at(2,2) ? 2 : 1) :
					(at(0,0) < at(2,2) ? 2 : 0);
				int j = (i + 1) % 3;
				int k = (i + 2) % 3;

				float s = std::sqrt(at(i,i) - at(j,j) - at(k,k) + 1.0f);
				temp.m[i] = s * 0.5f;
				s = 0.5f / s;

				temp.m[3] = (at(j,k) - at(k,j)) * s;
				temp.m[j] = (at(i,j) + at(j,i)) * s;
				temp.m[k] = (at(i,k) + at(k,i)) * s;
			}

			return (Quaternion)temp;
		}

		void setEuler( const vec<3,T>& v )
		{
			float ci = std::cos(v.roll);
			float cj = std::cos(v.yaw);
			float ch = std::cos(v.pitch);
			float si = std::sin(v.roll);
			float sj = std::sin(v.yaw);
			float sh = std::sin(v.pitch);
			float cc = ci * ch;
			float cs = ci * sh;
			float sc = si * ch;
			float ss = si * sh;

			at(0,0) = cj * ch;
			at(1,0) = sj * sc - cs;
			at(2,0) = sj * cc + ss;

			at(0,1) = cj * sh;
			at(1,1) = sj * ss + cc;
			at(2,1) = sj * cs - sc;

			at(0,2) = -sj;
			at(1,2) = cj * si;
			at(2,2) = cj * ci;
		}

		vec<3,T> getEuler() const
		{
			// first use the normal calculus
			vec<3,T> r(
				std::asin(-at(0,2)),
				std::atan2(at(0,1), at(0,0)),
				std::atan2(at(1,2), at(2,2))
			);

			// on pitch = +/-HalfPI
			// TODO: Fix this.
			/*
			if(std::abs(r.pitch) == SIMD_HALF_PI)
			{
				if(r.yaw > 0)
					r.yaw -= SIMD_PI;
				else
					r.yaw += SIMD_PI;

				if(r.roll > 0)
					r.roll -= SIMD_PI;
				else
					r.roll += SIMD_PI;
			}
			*/

			return r;
		};

		TMAT getInverse() const
		{
			mat<3,3,T> r;
			TMAT n;

			// Invert rotation
			for( int x = 0; x < 3; x++ )
				for( int y = 0; y < 3; y++ )
					n.at(x,y) = r.at(x,y) = at(y,x);

			// Invert position
			vec<3,T> t = -(r.transform(vec<3,T>( at(3,0), at(3,1), at(3,2) )));
			n.at(3,0) = t.x;
			n.at(3,1) = t.y;
			n.at(3,2) = t.z;

			//Fix some stuff
			n.at(0,3) = 0;
			n.at(1,3) = 0;
			n.at(2,3) = 0;
			n.at(3,3) = 1;

			return n;
		}
};


// #include <iostream>
// template< int X, int Y, class T >
// inline std::ostream& operator<<(std::ostream& o, const mat<X,Y,T>& m )
// {
// 	o.precision(0);
// 	o.setf(std::ios::fixed, std::ios::floatfield);
// 	o << "Matrix\n{\n";
// 	for( int y = 0; y < Y; y++ )
// 	{
// 		o << "\t[ ";
// 		for( int x = 0; x < X; x++ )
// 		{
// 			o << m.at(x,y) << " ";
// 		}
// 		o << "]\n";
// 	}
// 	o << "}";
// 	return o;
// }


#undef TMAT_RANGETEST
#undef TMAT

typedef mat<3,4,float> mat34f;

typedef mat<3,3,float> mat3f;
typedef mat<4,4,float> mat4f;

typedef mat<3,3,double> mat3d;
typedef mat<4,4,double> mat4d;

}

#endif