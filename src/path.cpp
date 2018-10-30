#include "path.hpp"
#include <string>

void parsePath(std::string str, std::string& path, std::string& filename) {
	path = "";
	filename = "";
	for(int i = 0; i < (int)str.size(); i++) {
		char c = str[i];
		filename += c;
		if (c == '/' || c == '\\') {
			path = path + filename;
			filename = "";
		}
	}
}