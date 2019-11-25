#ifndef MATH_DEFINES_HPP
#define MATH_DEFINES_HPP

#include "math/fixed_point.hpp"
#include "math/vec3.hpp"

#include <iostream>

// using scalar_t = math::fixed_point<16, 16>;
using scalar_t = float;
using vector_t = math::vec3<scalar_t>;
using colour_t = vector_t;

constexpr scalar_t PI = 3.14159265359;

template <size_t I, size_t F>
inline std::istream& operator>>( std::istream& in, math::fixed_point<I, F>& fp )
{
	double d;
	in >> d;
	fp = d;
	return in;
}

template <size_t I, size_t F>
inline std::ostream& operator<<( std::ostream& out, const math::fixed_point<I, F>& fp )
{
	double d = static_cast<double>( fp );
	out << d;
	return out;
}

template <typename T>
inline std::istream& operator>>( std::istream& in, math::vec3<T>& vec )
{
	in >> vec.x >> vec.y >> vec.z;
	return in;
}

template <typename T>
inline std::ostream& operator<<( std::ostream& out, const math::vec3<T>& vec )
{
	out << vec.x << ' ' << vec.y << ' ' << vec.z;
	return out;
}

#endif