#ifndef VEC3_HPP
#define VEC3_HPP

#include "fixed_point.hpp"

#include <cmath>

namespace math
{

template <class T>
class vec3
{
public:

	using value_type = T;

public: // construction

	vec3() = default;
	constexpr vec3( const T& v ) : x( v ), y( v ), z( v ) {}
	constexpr vec3( const T& x_, const T& y_, const T& z_ ) : x( x_ ), y( y_ ), z( z_ ) {}
	constexpr vec3( const vec3& ) = default;

public: // math

	constexpr vec3& operator+=( const vec3& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	constexpr vec3& operator-=( const vec3& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <typename SCALAR>
	constexpr vec3& operator*=( const SCALAR& s )
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	template <typename SCALAR>
	constexpr vec3& operator/=( const SCALAR& s )
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	friend constexpr vec3 operator+( vec3 lhs, const vec3& rhs )
	{
		return lhs += rhs;
	}

	friend constexpr vec3 operator-( vec3 lhs, const vec3& rhs )
	{
		return lhs -= rhs;
	}

	template <typename SCALAR>
	friend constexpr vec3 operator*( vec3 v, const SCALAR& s )
	{
		return v *= s;
	}

	template <typename SCALAR>
	friend constexpr vec3 operator*( const SCALAR& s, vec3 v )
	{
		return v *= s;
	}

	template <typename SCALAR>
	friend constexpr vec3 operator/( vec3 v, const SCALAR& s )
	{
		return v /= s;
	}

	double length() const
	{
		return std::sqrt( static_cast<double>( ( x * x ) + ( y * y ) + ( z * z ) ) );
	}

	void normalize()
	{
		auto l = length();
		if ( l == 0 )
			return;

		x /= l;
		y /= l;
		z /= l;
	}

public: // access

	T* data() { return ( T* )( char* )this; }
	const T* data() const { return ( const T* )( const char* )this; }

	T& operator[]( size_t component )
	{
		return data()[ component ];
	}

	const T& operator[]( size_t component ) const
	{
		return data()[ component ];
	}

public: // helper functions

	static vec3 normalize( vec3 v )
	{
		return v /= v.length();
	}

	static constexpr T dot_product( const vec3& lhs, const vec3& rhs )
	{
		return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y ) + ( lhs.z * rhs.z );
	}

	static constexpr vec3 cross_product( const vec3& lhs, const vec3& rhs )
	{
		return vec3(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x );
	}

	static constexpr vec3 project( const vec3& v, const vec3& onto )
	{
		return onto * dot_product( v, onto ) / dot_product( onto, onto );
	}

	static constexpr vec3 refract( const vec3& v, const vec3& normal, T index1, T index2 )
	{
		vec3 proj = project( v, normal );
		vec3 ortho = v - proj;
		return normalize( proj + ( index1 / index2 ) * ortho );
	}

	static double angle_between( const vec3& v1, const vec3& v2 )
	{
		return std::acos( vec3::dotProduct( v1, v2 ) / ( v1.length() * v2.length() ) );
	}

	static vec3 multiply( const vec3& lhs, const vec3& rhs )
	{
		return vec3( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
	}

	T x;
	T y;
	T z;
};

} // namespace math

#endif