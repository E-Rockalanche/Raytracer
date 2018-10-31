#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "vec3.hpp"

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
	Vec3 samplePixel(int x, int y) const;
	Vec3 sampleColour(float x, float y, SampleMode sample_mode = LINEAR, EdgeMode edge_mode = REPEAT) const;

private:
	int width, height, channels;
	unsigned char* data;
};

#endif