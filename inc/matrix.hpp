#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vec3.hpp"
#include "vec4.hpp"

class Matrix {
public:
	static const int SIZE = 4;

	Matrix();
	Matrix(float data[]);
	Matrix(const Matrix& other);

	float& operator[](int index);
	float operator[](int index) const;

	float get(int row, int col);
	void set(int row, int col, float value);

	Matrix& operator*=(float scalar);
	Matrix& operator/=(float scalar);

	friend Matrix operator*(float scalar, const Matrix& matrix);
	friend Matrix operator/(const Matrix& matrix, float scalar);

	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);
	Matrix& operator*=(const Matrix& other);

	friend Matrix operator*(const Matrix& m1, const Matrix& m2);
	friend Matrix operator+(const Matrix& m1, const Matrix& m2);
	friend Matrix operator-(const Matrix& m1, const Matrix& m2);

	friend Vec3 operator*(const Matrix& matrix, const Vec3& v);
	friend Vec3 operator*(const Vec3& v, const Matrix& matrix);
	friend Vec4 operator*(const Matrix& matrix, const Vec4& v);
	friend Vec4 operator*(const Vec4& v, const Matrix& matrix);

private:
	float data[SIZE * SIZE];
};

#endif