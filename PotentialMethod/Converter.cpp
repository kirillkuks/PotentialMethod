#include"Convertor.h"

Convertor::Convertor(PotentialParams const& pp) : linear{ new LinearData() } {
	std::vector<int> production_points = pp.production_points;
	std::vector<int> consumption_points = pp.consumption_points;
	Matrix table = pp.table;

	size_t n_size = table.get_n(), m_size = table.get_m();

	linear->func = std::vector<double>(n_size * m_size);
	for (size_t i = 0, k = 0; i < n_size; ++i) {
		for (size_t j = 0; j < m_size; ++j) {
			linear->func[k++] = table[i][j];
		}
	}

	linear->sign = std::vector<bool>(n_size * m_size);
	for (size_t i = 0; i < linear->sign.size(); ++i) {
		linear->sign[i] = true;
	}

	linear->system = LinearProgramming::Limitations();
	for (size_t i = 1; i < n_size; ++i) {
		std::vector<double> vec(n_size * m_size + 1);
		for (size_t j = 0; j < m_size; ++j) {
			vec[i * m_size + j] = 1;
		}
		vec[n_size * m_size] = production_points[i];
		linear->system.add_limitations({ vec, LinearProgramming::LT::LT_EQ });
	}
	for (size_t j = 0; j < m_size; ++j) {
		std::vector<double> vec(n_size * m_size + 1);
		for (size_t i = 0; i < n_size; ++i) {
			vec[i * m_size + j] = 1;
		}
		vec[n_size * m_size] = consumption_points[j];
		linear->system.add_limitations({ vec, LinearProgramming::LT::LT_EQ });
	}

	std::cout << "\n/*****************************************************************************/\n";
	std::cout << "func:\n";
	for (auto elem : linear->func) {
		std::cout << elem << " ";
	}
	std::cout << "\nLimitations\n";
	for (size_t i = 0; i < linear->system.limitations.size(); ++i) {
		for (size_t j = 0; j < linear->system.limitations[i].first.size(); ++j) {
			std::cout << linear->system.limitations[i].first[j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "/*****************************************************************************/\n";

 }

Convertor::~Convertor() {
	delete linear;
}

LinearData* Convertor::get_data() const {
	return linear;
}

LinearProgramming::Limitations::Limitations() {}

void LinearProgramming::Limitations::add_limitations(std::pair<std::vector<double>, LT>&& limitation) {
	limitations.push_back(limitation);
}