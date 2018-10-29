#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include <iostream>
#include <exception>

#include "models.hpp"
#include "material.hpp"
#include "scene.hpp"
#include "vec3.hpp"
#include "light.hpp"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

Pixel pixel_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

Scene scene;

// ambient, diffuse, specular, ka, kd, ks, smoothness
Material red(Vec3(1, 0, 0), Vec3(1, 0, 0), Vec3(1, 1, 1), 0.1, 1.0, 1.0, 10.0);
Material green(Vec3(0, 1, 0), Vec3(0, 1, 0), Vec3(0.8, 1, 0.8), 0.1, 1.0, 0.5, 5.0);
Material blue(Vec3(0, 0, 1), Vec3(0, 0, 1), Vec3(0.5, 0.5, 1), 0.1, 1.0, 0.1, 1.0);

Sphere red_sphere(Vec3(2, 0, -10), 2, red);
Sphere green_sphere(Vec3(-3, 0, -12), 2, green);
Sphere blue_sphere(Vec3(0, 0, -14), 2, blue);

Light white_light(Vec3(0, 10, -5), Vec3(1, 1, 1));

void initializeScene() {
	scene.addModel(&red_sphere);
	scene.addModel(&green_sphere);
	scene.addModel(&blue_sphere);
	
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

