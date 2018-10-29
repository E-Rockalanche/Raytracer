#ifndef PIXEL_HPP
#define PIXEL_HPP

typedef unsigned char uchar;

class Pixel {
public:
	Pixel() : red(0), green(0), blue(0) {}
	Pixel(uchar r, uchar g, uchar b) : red(r), green(g), blue(b) {}

	uchar red;
	uchar green;
	uchar blue;
};

#endif