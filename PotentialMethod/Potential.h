#pragma once
#include <vector>
#include "Matrix.h"

struct PotentialParams {
	std::vector<int> production_points;
	std::vector<int> consumption_points;
	Matrix table;
};

class Potential {
public:
	Potential(PotentialParams const&);

	void northwest_coener_method();

	~Potential();
private:

	std::vector<int> production_points;
	std::vector<int> consumption_points;
	Matrix table;
};