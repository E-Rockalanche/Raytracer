#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "math_defines.hpp"

class Light
{
public:
	Light() = default;
	Light( vector_t p, colour_t c ) : position( p ), colour( c ) {}

	vector_t position{ 0 };
	colour_t colour{ 0 };
};

#endif
