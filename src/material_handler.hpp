#ifndef MATERIAL_HANDLER_HPP
#define MATERIAL_HANDLER_HPP

#include "material.hpp"
#include "path.hpp"

#include <string>
#include <unordered_map>
#include <vector>

class MaterialManager
{
public:
	
	static void initialize();
	static bool loadMaterialFile( const fs::path& filename );
	static int getHandle( std::string material_name );
	static const Material& getMaterial( int index );
	static int numMaterials() { return (int)materials.size(); }

private:

	static std::unordered_map<std::string, int> name_map;
	static std::vector<Material> materials;
	static Material default_material;
};

#endif