#ifndef LIGHT_HPP
#define LIGHT_HPP

class Light {
public:
	Light(Vec3 position, Vec3 colour) : position(position), colour(colour) {}

	Vec3 position;
	Vec3 colour;
};

#endif
