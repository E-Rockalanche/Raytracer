#ifndef VEC3_HPP
#define VEC3_HPP

#include <ostream>
#include <istream>

class Vec3 {
public:
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(const Vec3& other);
	Vec3& operator+=(Vec3 other);
	Vec3& operator-=(Vec3 other);
	Vec3& operator*=(Vec3 other);
	Vec3& operator*=(float scalar);
	Vec3& operator/=(float scalar);
	Vec3& normalize();
	friend Vec3 operator+(Vec3 v1, Vec3 v2);
	friend Vec3 operator-(Vec3 v1, Vec3 v2);
	friend Vec3 operator*(Vec3 v1, Vec3 v2);
	friend Vec3 operator*(float scalar, Vec3 v2);
	friend Vec3 operator/(Vec3 v1, float scalar);
	float& operator[](int index);
	float operator[](int index) const;
	float length() const;
	static float dotProduct(Vec3 v1, Vec3 v2);
	static Vec3 crossProduct(Vec3 v1, Vec3 v2);
	static Vec3 normalize(Vec3 v);
	static Vec3 project(Vec3 v, Vec3 onto);
	static Vec3 refract(Vec3 v, Vec3 normal, float index1, float index2);
	static float angleBetween(Vec3 v1, Vec3 v2);
	friend std::ostream& operator<<(std::ostream& out, Vec3 v);
	friend std::istream& operator>>(std::istream& in, Vec3& v);

	float x, y, z;
};

#endif
