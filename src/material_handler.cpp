
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "material_handler.hpp"
#include "vec3.hpp"
#include "material.hpp"
#include "path.hpp"
#include "texture_handler.hpp"

std::map<std::string, int> MaterialHandler::name_map;
std::vector<Material> MaterialHandler::materials;

void MaterialHandler::initialize() {
	materials.clear();
	name_map.clear();

	Vec3 purple(1, 0, 1);

	materials.push_back(Material(purple, purple, purple, 0));
}

int MaterialHandler::numMaterials() {
	return materials.size();
}

bool MaterialHandler::loadMaterialFile(std::string filename, std::string path) {
	parsePath(path + filename, path, filename);

	std::cout << "loading material " << filename << '\n';

	bool ok = false;
	std::ifstream fin((path + filename).c_str());
	if (fin.is_open()) {
		Material ignore;
		Material* cur_material = &ignore;
		while(!fin.fail()) {
			std::string str;
			fin >> str;
			if (str.size() > 0) {
				if (str == "newmtl") {
					std::string name;
					fin >> name;
					materials.push_back(Material());
					cur_material = &materials.back();
					cur_material->name = name;
					name_map[name] = materials.size()-1;
				} else if (str == "Ka") {
					// ambient colour
					fin >> cur_material->ambient;
				} else if (str == "Kd") {
					// diffuse colour
					fin >> cur_material->diffuse;
				} else if (str == "Ks") {
					// specular colour
					fin >> cur_material->specular;
				} else if (str == "Ns") {
					// specular exponent
					fin >> cur_material->specular_exponent;
				} else if (str == "map_Ka") {
					// ambient texture filename
					fin >> cur_material->ambient_map_filename;
				} else if (str == "map_Kd") {
					// ambient texture filename
					std::cout << "map_Kd\n";

					fin >> cur_material->diffuse_map_filename;
					int handle = TextureHandler::loadTextureFile(path + cur_material->diffuse_map_filename);
					if (handle == -1) {
						std::cout << "failed to load texture " << cur_material->diffuse_map_filename << '\n';
						break;
					} else {
						cur_material->diffuse_tex_handle = handle;
					}

					std::cout << "loaded texture\n";
				} else if (str == "map_Ks") {
					// ambient texture filename
					fin >> cur_material->specular_map_filename;
				} else if (str == "d") {
					fin >> cur_material->alpha;
				} else if (str == "Tf") {
					fin >> cur_material->transmission_filter;
				} else if (str == "Ni") {
					fin >> cur_material->refraction_index;
				} else {
					std::getline(fin, str);
				}
			}
		}
		ok = fin.eof();
		if (!ok) {
			std::cout << "failed to load " << filename << '\n';
		}
	} else {
		std::cout << "cannot open " << filename << '\n';
	}
	return ok;
}

int MaterialHandler::getHandle(std::string material_name) {
	auto it = name_map.find(material_name);
	if (it != name_map.end()) {
		return it->second;
	} else {
		return 0;
	}
}

Material& MaterialHandler::getMaterial(int index) {
	return materials[index];
}