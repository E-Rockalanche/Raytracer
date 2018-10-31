#include <string>
#include <istream>
#include "material.hpp"

Material::Material() {
	specular_exponent = 1.0;
	alpha = 1.0;
	refraction_index = 1.5;
	ambient_tex_handle = -1;
	diffuse_tex_handle = -1;
	specular_tex_handle = -1;
}

Material::Material(Vec3 ambient, Vec3 diffuse, Vec3 specular, float specular_exponent)
		: ambient(ambient), diffuse(diffuse), specular(specular),
		specular_exponent(specular_exponent) {
	alpha = 1.0;
	refraction_index = 1.5;
	ambient_tex_handle = -1;
	diffuse_tex_handle = -1;
	specular_tex_handle = -1;
}

std::ostream& operator<<(std::ostream& out, const Material& mat) {
	out << "Ka "<< mat.ambient << '\n';
	out << "Kd "<< mat.diffuse << '\n';
	out << "Ks "<< mat.specular << '\n';
	out << "Tf "<< mat.transmission_filter << '\n';

	out << "Ns "<< mat.specular_exponent << '\n';

	if (mat.ambient_map_filename.size() > 0) {
		out << "map_Ka "<< mat.ambient_map_filename << '\n';
	}

	if (mat.diffuse_map_filename.size() > 0) {
		out << "map_Kd "<< mat.diffuse_map_filename << '\n';
	}

	if (mat.specular_map_filename.size() > 0) {
		out << "map_Ks "<< mat.specular_map_filename << '\n';
	}

	out << "d "<< mat.alpha << '\n';
	out << "Ni "<< mat.refraction_index << '\n';

	return out;
}