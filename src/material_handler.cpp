
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "material_handler.hpp"

std::map<std::string, int> MaterialHandler::name_map;
std::vector<Material> MaterialHandler::materials;

void MaterialHandler::initialize() {
	materials.clear();
	name_map.clear();

	materials.push_back(Material(Vec3(0.1, 0.1, 0.1), Vec3(1, 1, 1), Vec3(1, 1, 1), 100));
}

int MaterialHandler::numMaterials() {
	return materials.size();
}

bool MaterialHandler::loadMaterialFile(std::string filename) {
	bool ok = false;
	std::ifstream fin(filename.c_str());
	if (fin.is_open()) {
		Material ignore;
		Material* cur_material = &ignore;
		while(!fin.fail()) {
			std::string str;
			fin >> str;
			if (str.size() > 0) {
				if (str == "newmtl") {
					std::string material_name;
					fin >> material_name;
					materials.push_back(Material());
					cur_material = &materials.back();
					name_map[material_name] = materials.size()-1;
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
					fin >> cur_material->diffuse_map_filename;
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
					if (str[0] != '#') {
						std::cout << "unknown token: " << str << '\n';
					}
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