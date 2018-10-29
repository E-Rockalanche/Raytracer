#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <istream>
#include <ostream>
#include "vec3.hpp"

struct Material {
	// colours
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	
	// weights
	float ka; // ambient weight
	float kd; // diffuse weight
	float ks; // specular weight
	float smoothness; // specular smoothness (higher number acts more like a mirror)
	
	/*
	float opacity = 1.0;
	float refraction_index = 1.5;
	*/

	Material() {
		ka = 0.1;
		kd = 1.0;
		ks = 0.5;
		smoothness = 10.0;
	}
	Material(Vec3 ambient, Vec3 diffuse, Vec3 specular, float ka, float kd, float ks, float smoothness)
		: ambient(ambient), diffuse(diffuse), specular(specular), ka(ka), kd(kd), ks(ks), smoothness(smoothness) {}

	friend std::ostream& operator<<(std::ostream& out, Material mat) {
		out << mat.ambient << ' ' << mat.diffuse << ' ' << mat.specular << ' '
			<< mat.ka << ' ' << mat.kd << ' ' << mat.ks << ' ' << mat.smoothness;
		return out;
	}

	friend std::istream& operator>>(std::istream& in, Material& mat) {
		in >> mat.ambient >> mat.diffuse >> mat.specular >> mat.ka >> mat.kd
			>> mat.ks >> mat.smoothness;
		return in;
	}
};

#endif