#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include <iostream>
#include <exception>
#include <fstream>

#include "models.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "vec3.hpp"
#include "light.hpp"
#include "material_handler.hpp"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

Pixel pixel_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

Scene scene;

void renderImage() {
	scene.render(SCREEN_WIDTH, SCREEN_HEIGHT, pixel_buffer);
}

void clearImage() {
	try {
		for( int x = 0; x < SCREEN_WIDTH; x++ ) {
			for( int y = 0; y < SCREEN_HEIGHT; y++ ) {
				int bufferInd = y*SCREEN_WIDTH + x;
				pixel_buffer[ bufferInd ] = Pixel(0, 0, 0);
			}
		}
	} catch (...) {
		std::cout << "Caught exception when clearing buffer\n";
	}
}

void keyboard(unsigned char key, int x, int y)  {
	switch(key) {
		case 'r':
			clearImage();
			renderImage();
			break;

		case 'm':
			for(int i = 1; i < MaterialHandler)
	}
}

void renderScene(void)  {
	glDrawPixels( SCREEN_WIDTH, SCREEN_HEIGHT,  GL_RGB, GL_UNSIGNED_BYTE, pixel_buffer );
	glutSwapBuffers();
}

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
		}
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutCreateWindow("Assignment 2 | Eric Roberts");

	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutKeyboardFunc( keyboard );
	
	glutMainLoop();

	return(0);
}
