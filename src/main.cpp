#include <iostream>
#include <exception>
#include <fstream>

#include "model.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "vec3.hpp"
#include "light.hpp"
#include "material_handler.hpp"
#include "texture_handler.hpp"
#include "path.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

Pixel pixel_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
Scene scene;

void clearImage() {
	for( int x = 0; x < SCREEN_WIDTH; x++ ) {
		for( int y = 0; y < SCREEN_HEIGHT; y++ ) {
			int rgb = ((x/8 + y/8) % 2) ? 255 : 0;
			pixel_buffer[y*SCREEN_WIDTH + x] = Pixel(rgb, rgb, rgb);
		}
	}
}

void keyboard(unsigned char key, int x, int y)  {
	switch(key) {
		case 'r':
			clearImage();
			scene.render(SCREEN_WIDTH, SCREEN_HEIGHT, pixel_buffer);
			break;
	}
}

void renderScene(void)  {
	glDrawPixels( SCREEN_WIDTH, SCREEN_HEIGHT,  GL_RGB, GL_UNSIGNED_BYTE, pixel_buffer );
	glutSwapBuffers();
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

	TextureHandler::clear();

	return(0);
}
