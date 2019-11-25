#ifndef vec4_HPP
#define vec4_HPP

namespace math
{

class vec4
{
public:

	using value_type = float;

public: // constructors

	vec4() = default;
	
	constexpr vec4( value_type x_, value_type y_, value_type z_, value_type w_ )
		: x( x_ ), y( y_ ), z( z_ ), w( w_ )
	{}

	constexpr vec4( const vec4& ) = default;

public: // assignment

	vec4& operator=( const vec4& ) = default;

public: // access

	value_type* data()
	{
		return ( value_type* )( char* )this;
	}

	const value_type* data() const
	{
		return ( const value_type* )( const char* )this;
	}

	value_type& operator[]( unsigned int index )
	{
		return data()[ index ];
	}

	const value_type& operator[]( unsigned int index ) const
	{
		return data()[ index ];
	}

public: // math

	vec4& operator+=( const vec4& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	vec4& operator-=( const vec4& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	template <typename SCALAR>
	vec4& operator*=( const SCALAR& s )
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	template <typename SCALAR>
	vec4& operator/=( const SCALAR& s )
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

public:

	value_type x;
	value_type y;
	value_type z;
	value_type w;
};

inline vec4 operator+( vec4 lhs, const vec4& rhs )
{
	return lhs += rhs;
}

inline vec4 operator-( vec4 lhs, const vec4& rhs )
{
	return lhs -= rhs;
}

template <typename SCALAR>
inline vec4 operator*( vec4 v, const SCALAR& s )
{
	return v *= s;
}

template <typename SCALAR>
inline vec4 operator*( const SCALAR& s, vec4 v )
{
	return v *= s;
}

template <typename SCALAR>
inline vec4 operator/( vec4 v, const SCALAR& s )
{
	return v /= s;
}

} // namespace math

#endif