#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "vec4.hpp"

class Texture {
public:
	enum SampleMode {
		NEAREST,
		LINEAR
	};
	enum EdgeMode {
		REPEAT,
		MIRROR_REPEAT,
		CLAMP,
		MIRROR_CLAMP
	};

	Texture(unsigned char* data, int width, int height, int channels);
	Vec4 samplePixel(int x, int y) const;
	Vec4 sampleColour(float x, float y, SampleMode sample_mode = LINEAR, EdgeMode edge_mode = REPEAT) const;

	int width, height, channels;
	unsigned char* data;
};

#endif