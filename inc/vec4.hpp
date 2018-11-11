#ifndef VEC4_HPP
#define VEC4_HPP

class Vec4 {
public:
	Vec4();
	Vec4(float x, float y, float z, float w);
	float& operator[](int index);
	float operator[](int index) const;
	Vec4& operator+=(Vec4 other);
	Vec4& operator-=(Vec4 other);
	Vec4& operator*=(Vec4 other);
	Vec4& operator*=(float scalar);
	Vec4& operator/=(float scalar);
	friend Vec4 operator*(float scalar, Vec4 v);
	friend Vec4 operator/(Vec4 v, float scalar);
	friend Vec4 operator+(Vec4 v1, Vec4 v2);
	friend Vec4 operator-(Vec4 v1, Vec4 v2);

	float x, y, z, w;
};

#endif