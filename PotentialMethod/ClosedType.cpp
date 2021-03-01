#include "ClosedType.h"

ClosedType::ClosedType(PotentialParams const& pp) : params{ pp } {
	int num1 = 0, num2 = 0;

	for (auto prod : params.production_points) {
		num1 += prod;
	}
	for (auto cons : params.consumption_points) {
		num2 += cons;
	}

	if (num1 == num2) {
		return;
	}
	if (num1 > num2) {
		product_more(num1 - num2, pp.table);
		return;
	}
	consum_more(num2 - num1, pp.table);
}

ClosedType::~ClosedType() {}

void ClosedType::product_more(int div, Matrix const& table) {
	params.consumption_points.push_back(div);
	params.fic = Fictitious::FIC_CONSUMPTION;

	size_t n_size = table.get_n();
	size_t m_size = table.get_m();

	params.table = Matrix(n_size + 1, m_size);
	for (size_t i = 0; i < n_size; ++i) {
		for (size_t j = 0; j < m_size; ++j) {
			params.table[i][j] = table[i][j];
		}
	}
}

void ClosedType::consum_more(int div, Matrix const& table) {
	params.fic = Fictitious::FIC_PRODUCTION;
	params.production_points.push_back(div);
	
	size_t n_size = table.get_n();
	size_t m_size = table.get_m();

	params.table = Matrix(n_size, m_size + 1);
	for (size_t i = 0; i < n_size; ++i) {
		for (size_t j = 0; j < m_size; ++j) {
			params.table[i][j] = table[i][j];
		}
	}
}

PotentialParams const& ClosedType::get_params() const {
	return params;
}