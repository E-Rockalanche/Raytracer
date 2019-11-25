
#include "debug.hpp"
#include "material_handler.hpp"
#include "material.hpp"
#include "math_defines.hpp"
#include "texture_handler.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

constexpr vector_t purple{ 1, 0, 1 };

std::unordered_map<std::string, int> MaterialManager::name_map;
std::vector<Material> MaterialManager::materials;
Material MaterialManager::default_material{ purple, purple, purple, 0 };

void MaterialManager::initialize()
{
	materials.clear();
	name_map.clear();
}

int MaterialManager::numMaterials()
{
	return materials.size();
}

bool loadTexture( const fs::path& filename, int& dest_handle )
{
	int handle = TextureHandler::loadTextureFile( filename );

	if ( handle == -1 )
		dbLogError("failed to load texture %s", filename.string().c_str() )
	else
		dest_handle = handle;

	return ( handle != -1 );
}

bool MaterialManager::loadMaterialFile( const fs::path& filename )
{
	bool ok = false;
	std::ifstream fin( filename.string().c_str() );
	if ( fin.is_open() )
	{
		Material ignore;
		Material* cur_material = &ignore;
		while ( !fin.fail() )
		{
			std::string str;
			fin >> str;
			if ( str.size() > 0 )
			{
				if ( str == "newmtl" )
				{
					std::string name;
					fin >> name;
					materials.push_back( Material() );
					cur_material = &materials.back();
					cur_material->name = name;
					name_map[ name ] = materials.size() - 1;
				}
				else if ( str == "Ka" )
				{
					// ambient colour
					fin >> cur_material->ambient;
				}
				else if ( str == "Kd" )
				{
					// diffuse colour
					fin >> cur_material->diffuse;
				}
				else if ( str == "Ks" )
				{
					// specular colour
					fin >> cur_material->specular;
				}
				else if ( str == "Ns" )
				{
					// specular exponent
					fin >> cur_material->specular_exponent;
				}
				else if ( str == "map_Ka" )
				{
					// ambient texture filename
					fin >> cur_material->ambient_map_filename;
					bool loaded = loadTexture( filename.parent_path() / cur_material->ambient_map_filename,
											   cur_material->ambient_tex_handle );
					if ( !loaded ) break;
				}
				else if ( str == "map_Kd" )
				{
					// diffuse texture filename
					fin >> cur_material->diffuse_map_filename;
					bool loaded = loadTexture( filename.parent_path() / cur_material->diffuse_map_filename,
											   cur_material->diffuse_tex_handle );
					if ( !loaded ) break;
				}
				else if ( str == "map_Ks" )
				{
					// specular texture filename
					fin >> cur_material->specular_map_filename;
					bool loaded = loadTexture( filename.parent_path() / cur_material->specular_map_filename,
											   cur_material->specular_tex_handle );
					if ( !loaded ) break;
				}
				else if ( str == "map_Kn" )
				{
					// specular texture filename
					fin >> cur_material->normal_map_filename;
					bool loaded = loadTexture( filename.parent_path() / cur_material->normal_map_filename,
											   cur_material->normal_tex_handle );
					if ( !loaded ) break;
				}
				else if ( str == "d" )
				{
					fin >> cur_material->alpha;
				}
				else if ( str == "Tf" )
				{
					fin >> cur_material->transmission_filter;
				}
				else if ( str == "Ni" )
				{
					fin >> cur_material->refraction_index;
				}
				else
				{
					std::getline( fin, str );
				}
			}
		}
		ok = fin.eof();
		if ( !ok )
		{
			std::cout << "failed to load " << filename << '\n';
		}
	}
	else
	{
		std::cout << "cannot open " << filename << '\n';
	}
	return ok;
}

int MaterialManager::getHandle( std::string material_name )
{
	auto it = name_map.find( material_name );
	if ( it != name_map.end() )
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}

Material& MaterialManager::getMaterial( int index )
{
	if ( index >= 0 && index < (int)materials.size() )
		return materials[ index ];
	else
		return default_material;
}