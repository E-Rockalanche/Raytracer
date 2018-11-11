
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include "pixel.hpp"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

Pixel pixel_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void renderImage() {
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for(int y = 0; y < SCREEN_HEIGHT; y++) {
			int rgb = ((x/8 + y/8) % 2) ? 255 : 0;
			pixel_buffer[x + y*SCREEN_WIDTH] = Pixel(rgb, rgb, rgb);
		}
	}
}

void clearImage() {
	for( int x = 0; x < SCREEN_WIDTH; x++ ) {
		for( int y = 0; y < SCREEN_HEIGHT; y++ ) {
			int bufferInd = y*SCREEN_WIDTH + x;
			pixel_buffer[ bufferInd ] = Pixel(0, 0, 0);
		}
	}
}

void keyboard(unsigned char key, int x, int y)  {
	switch(key) {
		case 'r':
			clearImage();
			renderImage();
			break;
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
	glutCreateWindow("Windows test");

	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutKeyboardFunc( keyboard );
	
	glutMainLoop();

	return(0);
}