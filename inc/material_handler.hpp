#ifndef MATERIAL_HANDLER_HPP
#define MATERIAL_HANDLER_HPP

#include <string>
#include <map>
#include <vector>
#include "material.hpp"

class MaterialHandler {
public:
	static void initialize();
	static bool loadMaterialFile(std::string filename);
	static int getHandle(std::string material_name);
	static Material& getMaterial(int index);
	static int numMaterials();
private:
	static std::map<std::string, int> name_map;
	static std::vector<Material> materials;
};

#endif