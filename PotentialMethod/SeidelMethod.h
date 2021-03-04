#pragma once
#include <cmath>
#include "Linear.h"

namespace LinearProgramming {

	struct Matrix;

	struct SeidelMethod {
	public:
		static std::vector<double> solve(LinearProgramming::Matrix&, std::vector<double>&);
	private:
		static LinearProgramming::Matrix multy(LinearProgramming::Matrix&, LinearProgramming::Matrix&);
		static std::vector<double> b_create(LinearProgramming::Matrix&, std::vector<double>&);
		static double norm(std::vector<double>&);
	};

}