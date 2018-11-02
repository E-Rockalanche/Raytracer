#include <iostream>
#include <exception>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "models.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "vec3.hpp"
#include "light.hpp"
#include "material_handler.hpp"
#include "path.hpp"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 64

Pixel pixel_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

Scene scene;

const char* gradient = " .,:;X$#";
const int gradient_length = 8;

int main(int argc, char* argv[]) {
	MaterialHandler::initialize();

	if (argc != 2) {
		std::cout << "requires scene filename\n";
		return 1;
	} else {
		std::string path;
		std::string filename;
		parsePath(argv[1], path, filename);
		bool ok = scene.loadScene(filename, path);
		if (!ok) {
			std::cout << "failed to load scene\n";
			return 1;
		} else {
			std::cout << "models: " << scene.models.size() << '\n';
			std::cout << "lights: " << scene.lights.size() << '\n';
			std::cout << "materials: " << MaterialHandler::numMaterials() << '\n';
		}
	}

	scene.render(SCREEN_WIDTH, SCREEN_HEIGHT, pixel_buffer);

	#define clamp(v, l, h) (((v) < (l)) ? (l) : (((v) > (h)) ? (h) : (v)))

	for(int y = SCREEN_HEIGHT-1; y >= 0; y--) {
		for(int x = 0; x < SCREEN_WIDTH; x++) {
			const Pixel& p = pixel_buffer[x + y*SCREEN_WIDTH];
			float illumination = (0.299 * p.red + 0.587 * p.green +  0.114 * p.blue)/255.0;
			int raw_index = std::round(illumination * gradient_length);
			int gradient_index = clamp(raw_index, 0, gradient_length-1);
			std::cout << gradient[gradient_index] << ' ';
		}
		std::cout << '\n';
	}

	return 0;
}
