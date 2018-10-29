#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vec3.hpp"

class Matrix {
public:
	Matrix();
	Matrix(const Matrix& other);
	Matrix(float* data);

	Matrix& operator*=(const Matrix& other);
	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);

	float& operator[](int index);
	float operator[](int index) const;

	friend Matrix operator+(const Matrix& m1, const Matrix& m2);
	friend Matrix operator-(const Matrix& m1, const Matrix& m2);
	friend Matrix operator*(const Matrix& m1, const Matrix& m2);
	friend Vec4 operator*(const Matrix& m, Vec4 v);

	Matrix& invert();

	static Matrix getInverse(const Matrix& m);

private:
	float data[16];
};

#endif