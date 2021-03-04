#pragma once
#include <vector>
#include "Matrix.h"

class Conditions {
public:
	Conditions(std::vector<int>&, std::vector<int>&, std::vector<int>&, Matrix&);

	bool condition1() const;
	bool condition2() const;

	~Conditions();
private:

	std::vector<int> const c, x, y;
	Matrix const A;
};