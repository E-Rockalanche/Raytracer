#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <istream>
#include "vec3.hpp"

class Material {
public:
	Material();
	Material(Vec3 ambient, Vec3 diffuse, Vec3 specular, float specular_exponent);

	friend std::ostream& operator<<(std::ostream& out, const Material& mat);

	// colours
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 transmission_filter;

	float specular_exponent; // spectral exponent (smoothness) (higher number acts more like mirror)

	std::string ambient_map_filename;
	std::string diffuse_map_filename;
	std::string specular_map_filename;

	float alpha;
	float refraction_index;
};

#endif