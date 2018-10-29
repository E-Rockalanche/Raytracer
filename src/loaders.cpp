/*
loaders.cpp
Eric Roberts
*/

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <map>
#include "loaders.hpp"

bool loadMaterials(const std::string& filename, const std::string& path, Object& object) {
	std::ifstream fin(path + filename);
	if (!fin.is_open() || !fin.good()) {
		dout("could not open material file");
		return false;
	}

	Material* cur_material = NULL;

	while(!fin.eof()) {
		std::string str;
		fin >> str;
		if (str.size() > 0) {
			if (str[0] == '#') {
				std::getline(fin, str);
			} else if (str == "newmtl") {
				// new material
				std::string material_name;
				fin >> material_name;
				cur_material = &object.materials[material_name];
			} else if (str == "Ka") {
				// ambient colour
				for(int i = 0; i < 3; i++) {
					fin >> cur_material->ambient_colour[i];
				}
			} else if (str == "Kd") {
				// diffuse colour
				for(int i = 0; i < 3; i++) {
					fin >> cur_material->diffuse_colour[i];
				}
			} else if (str == "Ks") {
				// diffuse colour
				for(int i = 0; i < 3; i++) {
					fin >> cur_material->specular_colour[i];
				}
			} else if (str == "Ke") {
				// emissive colour
				for(int i = 0; i < 3; i++) {
					fin >> cur_material->emissive_colour[i];
				}
			} else if (str == "Ns") {
				// specular weight
				fin >> cur_material->specular_weight;
			} else if (str == "Ni") {
				// optical density
				fin >> cur_material->optical_density;
			} else if (str == "d") {
				fin >> cur_material->dissolve;
			} else if (str == "illum") {
				fin >> cur_material->illumination_mode;
			} else if (str == "Tf") {
				for(int i = 0; i < 3; i++) {
					fin >> cur_material->transmission_filter[i];
				}
			} else if (str == "map_Kd") {
				fin >> cur_material->diffuse_tex_file;
			} else if (str == "map_Ka") {
				fin >> cur_material->ambient_tex_file;
				return false;
			} else if (str == "map_Ks") {
				fin >> cur_material->specular_tex_file;
				return false;
			} else if (str == "map_Ns") {
				fin >> cur_material->highlight_tex_file;
				return false;
			} else if (str == "map_d") {
				fin >> cur_material->alpha_tex_file;
				return false;
			} else if (str == "map_bump" || str == "bump") {
				fin >> cur_material->bump_tex_file;
				return false;
			} else if (str == "disp") {
				std::cout << "displacement map not implemented\n";
				return false;
			} else if (str == "decal") {
				std::cout << "stencil decal texture map not implemented\n";
				return false;
			} else {
				std::cout << "invalid token: " << str << '\n';
				return false;
			}
		}

		if (fin.fail() && !fin.eof()) {
			std::cout << "material file data is invalid";
			return false;
		}
	}

	return true;
}

bool loadObject(const std::string& filename, const std::string& path, Object& object) {
	std::ifstream fin(path + filename);
	if (!fin.is_open() || !fin.good()) {
		std::cout << "could not open object file\n";
		return false;
	}

	PolygonGroup* cur_group = &object.polygon_groups[""];

	while(!fin.eof()) {
		std::string str;
		fin >> str;
		if (str.size()) {
			if (str[0] == '#') {
				// comment
				std::getline(fin, str);
			} else if (str == "v") {
				// vertex (x, y, z [,w]) w = 1.0
				GLfloat f;
				for(int i = 0; i < 3; i++) {
					fin >> f;
					object.minimums[i] = std::min(f, object.minimums[i]);
					object.maximums[i] = std::max(f, object.maximums[i]);
					object.vertices.push_back(f);
				}
			} else if (str == "vt") {
				// tex coord (u, v [,w]) w = 0.0
				GLfloat f;
				for(int i = 0; i < 2; i++) {
					fin >> f;
					if (i == 1) {
						f = 1.0 - f;
					}
					object.tex_coords.push_back(f);
				}
			} else if (str == "vn") {
				// normal (x, y, z)
				GLfloat f;
				for(int i = 0; i < 3; i++) {
					fin >> f;
					object.normals.push_back(f);
				}
			} else if (str == "vp") {
				// space vector
				std::cout << "space vectors not implemented\n";
				return false;
			} else if (str == "f") {
				// face
				for(int element = 0; element < 3; element++) {
					while(std::isspace(fin.peek())) fin.get();
					for(int type = 0; type < 3; type++) {
						if (fin.peek() != '/') {
							int n;
							fin >> n;
							n--;
							std::vector<unsigned int>* index_vec;
							std::vector<float>* float_vec;
							int vector_size = 0;
							switch(type) {
								case 0:
									index_vec = &cur_group->vertex_indices;
									float_vec = &object.vertices;
									vector_size = 3;
									break;
								
								case 1:
									index_vec = &cur_group->tex_coord_indices;
									float_vec = &object.tex_coords;
									vector_size = 2;
									break;
									
								case 2:
									index_vec = &cur_group->normal_indices;
									float_vec = &object.normals;
									vector_size = 3;
									break;
							}
							int num_vertices = float_vec->size()/vector_size;
							if (n < 0 || n >= num_vertices) {
								std::cout << "index " << n << " out of bounds " << num_vertices << '\n';
								return false;
							}
							index_vec->push_back(n);
						}
						if (fin.peek() == '/') {
							fin.get();
						}
					}
				}
			} else if (str == "l") {
				// polyline
				std::cout << "polyline not supported\n";
				return false;
			} else if (str == "mtllib") {
				// material file
				std::string material_filename;
				fin >> material_filename;
				bool success = loadMaterials(material_filename.c_str(), path, object);
				if (!success) {
					std::cout << "could not load material\n";
					return false;
				}
			} else if (str == "g") {
				// polygon group
				std::string group_name;
				fin >> group_name;
				cur_group = &object.polygon_groups[group_name];
			} else if (str == "s") {
				// smooth shading
				fin >> cur_group->smooth;
			} else if (str == "usemtl") {
				// use material from material file
				fin >> cur_group->material;
			} else if (str == "o") {
				// object name
				fin >> object.name;
			} else {
				std::cout << "invalid token: " << str << '\n';
				return false;
			}
		}

		if (fin.fail() && !fin.eof()) {
			std::cout << "object file data is invalid\n";
			return false;
		}
	}
	
	for(int i = 0; i < 3; i++) {
		object.origin[i] = (object.maximums[i] + object.minimums[i]) / 2;
	}

	return true;
}
