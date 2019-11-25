#ifndef TEXTURE_HANDLER_HPP
#define TEXTURE_HANDLER_HPP

#include "texture.hpp"
#include "path.hpp"

#include <string>
#include <map>
#include <vector>

class TextureHandler
{
public:
	static int loadTextureFile( const fs::path& filename );
	static int getHandle( const std::string& texture_name );
	static Texture& getTexture( int index );
	static int numTextures();
	static void clear();
private:
	static std::map<std::string, int> name_map;
	static std::vector<Texture> textures;
};

#endif