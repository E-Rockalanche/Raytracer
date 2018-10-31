#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "texture_handler.hpp"
#include "texture.hpp"
#include "stb_image.h"

std::map<std::string, int> TextureHandler::name_map;
std::vector<Texture> TextureHandler::textures;

int TextureHandler::loadTextureFile(std::string filename) {
	std::cout << "loading texture " << filename << '\n';
	int handle = getHandle(filename);
	if (handle == -1) {
		int width, height, channels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
		if (data == NULL) {
			handle = -1;
		} else {
			handle = textures.size();
			name_map[filename] = handle;
			textures.push_back(Texture(data, width, height, channels));
		}
	}
	return handle;
}

int TextureHandler::getHandle(std::string texture_name) {
	auto it = name_map.find(texture_name);
	if (it != name_map.end()) {
		return it->second;
	} else {
		return -1;
	}
}

Texture& TextureHandler::getTexture(int index) {
	if (index < 0 || index >= (int)textures.size()) {
		std::cout << "tex handle out of bounds!\n";
	}
	return textures[index];
}

int TextureHandler::numTextures() {
	return textures.size();
}