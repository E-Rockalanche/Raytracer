#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "math_defines.hpp"

// #include <iostream>

class Material
{
public:
	Material() = default;
	Material( colour_t ambient_, colour_t diffuse_, colour_t specular_, scalar_t specular_exponent_ )
		: ambient( ambient_ )
		, diffuse( diffuse_ )
		, specular( specular_ )
		, specular_exponent( specular_exponent_ )
	{}


	std::string name;

	// colours
	colour_t ambient{ 0 };
	colour_t diffuse{ 0 };
	colour_t specular{ 0 };
	colour_t transmission_filter{ 1, 1, 1 };

	scalar_t specular_exponent = 1.0f;

	std::string ambient_map_filename;
	std::string diffuse_map_filename;
	std::string specular_map_filename;
	std::string normal_map_filename;

	int ambient_tex_handle = -1;
	int diffuse_tex_handle = -1;
	int specular_tex_handle = -1;
	int normal_tex_handle = -1;

	scalar_t alpha = 1.0f;
	scalar_t refraction_index = 1.0f;
};

// std::ostream& operator<<( std::ostream& out, const Material& material );

#endif