#include "Conditions.h"

Conditions::Conditions(std::vector<int>& c, std::vector<int>& x, std::vector<int>& y, Matrix& A) : c{ c }, x{ x }, y{ y }, A{ A } {}

Conditions::~Conditions() {}

bool Conditions::condition1() const {
	for (auto elem : y) {
		if (elem < 0) {
			return false;
		}
	}
	return true;
}

bool Conditions::condition2() const {
	size_t n_size = A.get_n(), m_size = A.get_m();
	
	std::vector<int> tmp(m_size);
	
	for(size_t i = 0; i < m_size; ++i) {
		for (size_t j = 0; j < n_size; ++j) {
			tmp[i] += y[i] * A[i][j];
		}
	}

	std::vector<int> d(m_size);
	for (size_t i = 0; i < m_size; ++i) {
		d[i] = c[i] - tmp[i];
	}

	int res = 0;
	for (size_t i = 0; i < m_size; ++i) {
		res += d[i] * x[i];
	}

	return res == 0;
}