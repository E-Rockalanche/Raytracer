#include "matrix.hpp"

Matrix::Matrix() {
	// initialize to identity
	for(int i = 0; i < SIZE*SIZE; i++) {
		data[i] = (i%5 == 0);
	}
}
Matrix::Matrix(float data[]) {
	for(int i = 0; i < SIZE*SIZE; i++) {
		this->data[i] = data[i];
	}
}

Matrix::Matrix(const Matrix& other) {
	for(int i = 0; i < SIZE*SIZE; i++) {
		data[i] = other.data[i];
	}
}

float& Matrix::operator[](int index) {
	return data[index];
}

float Matrix::operator[](int index) const {
	return data[index];
}

float Matrix::get(int row, int col) {
	return data[row*SIZE + col];
}
void Matrix::set(int row, int col, float value) {
	data[row*SIZE + col] = value;
}

Matrix& Matrix::operator*=(float scalar) {
	for(int i = 0; i < SIZE*SIZE; i++) {
		data[i] *= scalar;
	}
	return *this;
}

Matrix& Matrix::operator/=(float scalar) {
	for(int i = 0; i < SIZE*SIZE; i++) {
		data[i] /= scalar;
	}
	return *this;
}

Matrix operator*(float scalar, const Matrix& matrix) {
	Matrix result;
	for(int i = 0; i < Matrix::SIZE*Matrix::SIZE; i++) {
		result.data[i] = matrix.data[i] * scalar;
	}
	return result;
}

Matrix operator/(const Matrix& matrix, float scalar) {
	Matrix result;
	for(int i = 0; i < Matrix::SIZE*Matrix::SIZE; i++) {
		result.data[i] = matrix.data[i] / scalar;
	}
	return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
	for(int i = 0; i < SIZE*SIZE; i++) {
		data[i] += other.data[i];
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
	for(int i = 0; i < SIZE*SIZE; i++) {
		data[i] -= other.data[i];
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
	// matrix multiplication
	Matrix temp = *this;
	for(int row = 0; row < SIZE; row++) {
		for(int col = 0; col < SIZE; col++) {
			float sum = 0;
			for(int i = 0; i < SIZE; i++) {
				sum += temp.data[row*SIZE + i] * other.data[i*SIZE + col];
			}
			data[row*SIZE + col] = sum;
		}
	}
	return *this;
}

Matrix operator*(const Matrix& m1, const Matrix& m2) {
	Matrix result;
	for(int row = 0; row < Matrix::SIZE; row++) {
		for(int col = 0; col < Matrix::SIZE; col++) {
			float sum = 0;
			for(int i = 0; i < Matrix::SIZE; i++) {
				sum += m1.data[row*Matrix::SIZE + i] * m2.data[i*Matrix::SIZE + col];
			}
			result.data[row*Matrix::SIZE + col] = sum;
		}
	}
	return result;
}

Matrix operator+(const Matrix& m1, const Matrix& m2) {
	Matrix result;
	for(int i = 0; i < Matrix::SIZE*Matrix::SIZE; i++) {
		result.data[i] = m1.data[i] + m2.data[i];
	}
	return result;
}

Matrix operator-(const Matrix& m1, const Matrix& m2) {
	Matrix result;
	for(int i = 0; i < Matrix::SIZE*Matrix::SIZE; i++) {
		result.data[i] = m1.data[i] - m2.data[i];
	}
	return result;
}