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

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

Pixel pixel_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

Scene scene;

Light white_light(Vec3(0, 10, -5), Vec3(1, 1, 1));

void initializeScene() {
	scene.addLight(&white_light);
}

void renderImage() {
	scene.render(SCREEN_WIDTH, SCREEN_HEIGHT, pixel_buffer);
}

void clearImage() {
	try {
		for( int x = 0; x < SCREEN_WIDTH; x++ ) {
			for( int y = 0; y < SCREEN_HEIGHT; y++ ) {	
				int bufferInd = y*SCREEN_WIDTH+x;
				pixel_buffer[ bufferInd ] = Pixel(0, 0, 0);
			}
		}
	} catch (...) {
		std::cout << "Caught exception when clearing buffer\n";
	}
}

void keyboard(unsigned char key, int x, int y)  {
	if( 'r' == key ) {
		clearImage();
		renderImage();
	}
}

void renderScene(void)  {
	glDrawPixels( SCREEN_WIDTH, SCREEN_HEIGHT,  GL_RGB, GL_UNSIGNED_BYTE, pixel_buffer );
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	int num_models;
	if (argc != 2) {
		std::cout << "requires scene filename\n";
		return 1;
	} else {
		std::ifstream fin(argv[1]);
		if (fin.is_open()) {
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

	std::cout << "models: " << num_models << '\n';

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutCreateWindow("Assignment 2");

	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutKeyboardFunc( keyboard );
	
	initializeScene();
	
	glutMainLoop();

	return(0);
}

