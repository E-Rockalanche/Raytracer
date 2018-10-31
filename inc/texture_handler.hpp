#ifndef TEXTURE_HANDLER_HPP
#define TEXTURE_HANDLER_HPP

#include <string>
#include <map>
#include <vector>
#include "texture.hpp"

class TextureHandler {
public:
	static int loadTextureFile(std::string filename);
	static int getHandle(std::string texture_name);
	static Texture& getTexture(int index);
	static int numTextures();
private:
	static std::map<std::string, int> name_map;
	static std::vector<Texture> textures;
};

#endif