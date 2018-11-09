#ifndef VEC4_HPP
#define VEC4_HPP

class Vec4 {
public:
	Vec4();
	Vec4(float x, float y, float z, float w);
	float& operator[](int index);
	float operator[](int index) const;

	float x, y, z, w;
};

#endif