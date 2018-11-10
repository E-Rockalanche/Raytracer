#include <stdexcept>
#include "vec4.hpp"

Vec4::Vec4() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vec4::Vec4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float& Vec4::operator[](int index) {
	switch(index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			throw std::runtime_error("Vec4 index out of bounds: "+std::to_string(index));
	}
}

float Vec4::operator[](int index) const {
	switch(index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			throw std::runtime_error("Vec4 index out of bounds: "+std::to_string(index));
	}
}

Vec4& Vec4::operator+=(Vec4 other){
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Vec4& Vec4::operator-=(Vec4 other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Vec4& Vec4::operator*=(Vec4 other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

Vec4 operator*(float scalar, Vec4 v) {
	Vec4 result;
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	result.w = v.w * scalar;
	return result;
}

Vec4 operator/(Vec4 v, float scalar) {
	Vec4 result;
	result.x = v.x / scalar;
	result.y = v.y / scalar;
	result.z = v.z / scalar;
	result.w = v.w / scalar;
	return result;
}