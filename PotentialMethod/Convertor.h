#pragma once
#include <iostream>
#include "Potential.h"
#include "Linear.h"

struct PotentialParams;

struct LinearData {
	std::vector<bool> sign;
	std::vector<double> func;
	LinearProgramming::Limitations system;
};

class Convertor {
public:
	Convertor(PotentialParams const&);

	LinearData* get_data() const;

	~Convertor();
private:
	LinearData* linear;
};