
#include "debug.hpp"
#include "texture_handler.hpp"
#include "texture.hpp"
#include "stb_image.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>

std::map<std::string, int> TextureHandler::name_map;
std::vector<Texture> TextureHandler::textures;

int TextureHandler::loadTextureFile( const fs::path& filename )
{
	auto filename_str = filename.filename().string();

	int handle = getHandle( filename_str );
	if ( handle != -1 )
		return handle;
	
	int width, height, channels;
	unsigned char* data = stbi_load( filename.c_str(), &width, &height, &channels, 0 );
	if ( data == nullptr )
	{
		dbLogError( "cannot load texture %s", filename.c_str() );
		return -1;
	}

	handle = textures.size();
	name_map[ filename_str ] = handle;
	textures.push_back( Texture( data, width, height, channels ) );
	
	return handle;
}

int TextureHandler::getHandle( const std::string& texture_name )
{
	auto it = name_map.find( texture_name );
	if ( it != name_map.end() )
	{
		return it->second;
	}
	else
	{
		return -1;
	}
}

const Texture& TextureHandler::getTexture( int index )
{
	dbAssertMessage( index >= 0 && index < (int)textures.size(), "invalid texture handle" );
	return textures[ index ];
}

int TextureHandler::numTextures()
{
	return textures.size();
}

void TextureHandler::clear()
{
	for ( int i = 0; i < ( int )textures.size(); i++ )
	{
		if ( textures[ i ].data )
		{
			delete[] textures[ i ].data;
		}
	}
	textures.clear();
}