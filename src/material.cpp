#include <string>
#include <istream>
#include "material.hpp"

Material::Material() {
	float specular_exponent = 1.0;
	float specular_weight = 1.0;
	float alpha = 1.0;
	float refraction_index = 1.0;
}

Material::Material(Vec3 ambient, Vec3 diffuse, Vec3 specular, float specular_exponent)
	: Material(), ambient(ambient), diffuse(diffuse), specular(specular),
	specular_exponent(specular_exponent) {}