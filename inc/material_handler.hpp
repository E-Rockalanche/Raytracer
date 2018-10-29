#ifndef MATERIAL_HANDLER_HPP
#define MATERIAL_HANDLER_HPP

#include <string>
#include <map>
#include <vector>

class MaterialHandler {
public:
	static void initialize();
	static bool loadMaterialFile(std::string filename);
	static int getHandle(std::string material_name);
private:
	static std::map<std::string, int> name_map;
	static std::vector<Material> materials;
};

#endif