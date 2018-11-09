#include <stdexcept>
#include <iostream>
#include "vec3.hpp"
#include "texture.hpp"

Texture::Texture(unsigned char* data, int width, int height, int channels)
		: width(width), height(height), channels(channels), data(data) {
	if (data == NULL) {
		throw std::runtime_error("texture data is NULL");
	}
	if (width == 0) {
		throw std::runtime_error("texture width is 0");
	}
	if (height == 0) {
		throw std::runtime_error("texture height is 0");
	}
	if (channels < 3) {
		throw std::runtime_error("texture channels is less than 3");
	}
}

#define clamp(a, low, high) (((a) < (low)) ? (low) : (((a) > (high)) ? (high) : (a)));

Vec3 Texture::samplePixel(int x, int y) const {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		throw std::runtime_error("texture sample position out of bounds");
	}
	unsigned char* p = data + (x + (height - y - 1)*width) * channels;
	return Vec3(p[0]/255.0, p[1]/255.0, p[2]/255.0);
}

Vec3 Texture::sampleColour(float x, float y, Texture::SampleMode sample_mode, Texture::EdgeMode edge_mode) const {
	Vec3 colour;

	switch(edge_mode) {
		default:
		case REPEAT:
			while (x > 1) {
				x -= 1;
			}
			while (x < 0) {
				x += 1;
			}
			while (y > 1) {
				y -= 1;
			}
			while (y < 0) {
				y += 1;
			}
			break;

		case CLAMP:
			x = clamp(x, 0, 1);
			y = clamp(y, 0, 1);
			break;
	}

	switch(sample_mode) {
		default:
		case LINEAR:
			{
				float fx, fy;
				fx = x*width - 0.5;
				fy = y*height - 0.5;

				int x1, y1, x2, y2;
				x1 = clamp(fx, 0, width-1);
				y1 = clamp(fy, 0, height-1);
				x2 = clamp(x1 + 1, 0, width-1);
				y2 = clamp(y1 + 1, 0, width-1);

				float w = x - x1;
				float h = fy - y1;

				colour += (1 - w) * (1 - h) * samplePixel(x1, y1);
				colour += w * (1 - h) * samplePixel(x2, y1);
				colour += (1 - w) * h * samplePixel(x1, y2);
				colour += w * h * samplePixel(x2, y2);
			}
			break;

		case NEAREST:
			{
				int px = clamp(x * width, 0, width-1);
				int py = clamp(y * height, 0, height-1);
				colour = samplePixel(px, py);
			}
			break;
	}

	return colour;
}