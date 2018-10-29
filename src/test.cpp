#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>

#include "scene.hpp"
#include "models.hpp"

char gradient[] =  " .,;xX$&@";
int gradient_length = 9;

int main(int argc, char** argv) {
	Scene scene;

	if (argc != 2) {
		std::cout << "requires scene filename\n";
		return 1;
	} else {
		std::ifstream fin(argv[1]);
		if (fin.is_open()) {
			int num_models;
			fin >> num_models;
			for(int i = 0; i < num_models; i++) {
				Model* model;
				fin >> model;
				if (model == NULL) {
					std::cout << "invalid model\n";
					return 1;
				} else {
					scene.addModel(model);
				}
			}
		}
		fin.close();
	}

	/*

	// ambient, diffuse, specular, ka, kd, ks, smoothness
	Material mat(Vec3(1, 1, 1), Vec3(1, 1, 1), Vec3(1, 1, 1), 0.2, 1.0, 0.5, 10.0);

	Sphere sphere1(Vec3(0, -5, -10), 2, mat);
	scene.addModel(&sphere1);

	RecPrism prism1(Vec3(-2, 0, -10), Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, -1), mat);
	scene.addModel(&prism1);

	RecPrism prism2(Vec3(0.5, 0, -10), Vec3(0, 0, -1), Vec3(0, 1, 0), Vec3(-1, 0, 0), mat);
	scene.addModel(&prism2);

	RecPrism prism3(Vec3(1, 0, -11), Vec3(1, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0), mat);
	scene.addModel(&prism3);

	*/

	Light light1(Vec3(0, 5, 5), Vec3(1, 1, 1));
	scene.addLight(&light1);

	const int width = 48;
	const int height = 48;

	Pixel buffer[width * height];


	scene.render(width, height, buffer);

	for(int x = 0; x < width + 2; x++) {
		std::cout << "# ";
	}
	std::cout << '\n';
	for(int y = height-1; y >= 0; y--) {
		std::cout << "# ";
		for(int x = 0; x < width; x++) {
			Pixel pixel = buffer[x + y * width];
			/*
			if (pixel.red > 0 || pixel.green > 0 || pixel.blue > 0) {
				std::cout << "X ";
			} else {
				std::cout << "  ";
			}
			*/
			float luminocity = (0.299*pixel.red + 0.587*pixel.green + 0.114*pixel.blue)/255.0;
			int gradient_index = std::min((int)std::round(luminocity*gradient_length), gradient_length-1);
			std::cout << gradient[gradient_index] << ' ';

		}
		std::cout << "#\n";
	}
	for(int x = 0; x < width + 2; x++) {
		std::cout << "# ";
	}
	std::cout << '\n';
}