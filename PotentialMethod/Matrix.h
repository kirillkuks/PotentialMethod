#pragma once
#include <iostream>

class Matrix {
private:
	struct Vector;
public:
	Matrix();
	Matrix(size_t, size_t);
	Matrix(Matrix const&);

	static Matrix create_matrix(size_t, size_t);

	Matrix& operator=(Matrix);
	Vector operator[](size_t);

	size_t get_n() const;
	size_t get_m() const;
	Matrix get_transposed() const;

	void print() const;

	~Matrix();
private:
	struct Vector {
		Vector(int*);
		int& operator[](size_t);
	private:
		int* vector;
	};

	int** matrix;
	size_t n, m;
};