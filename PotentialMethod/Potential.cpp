#include "Potential.h"

Potential::Potential(PotentialParams const& pp)
	: production_points{ pp.production_points },
	consumption_points{ pp.consumption_points },
	table{ pp.table } {
	std::cout << "production_points:\n";
	for (auto elem : production_points) {
		std::cout << elem << ' ';
	}
	std::cout << "\nconsumption_points:\n";
	for (auto elem : consumption_points) {
		std::cout << elem << ' ';
	}
	std::cout << "\nTable:\n";
	table.print();
}

void Potential::northwest_corner_method() {
	plan = Matrix::create_matrix(production_points.size(), consumption_points.size());
	for (size_t i = 0, j = 0; i < production_points.size() && j < consumption_points.size();) {
		int min = std::min(production_points[i], consumption_points[j]);

		plan[i][j] = min;
		production_points[i] -= min;
		consumption_points[j] -= min;

		if (consumption_points[j] == 0) {
			for (size_t k = i + 1; k < production_points.size(); ++k) {
				plan[k][j] = -1;
			}
			++j;
			continue;
		}
		else {
			for (size_t k = j + 1; k < consumption_points.size(); ++k) {
				plan[i][k] = -1;
			}
			++i;
			continue;
		}
	}
	std::cout << "\nplan:\n";
	plan.print();
}

size_t Potential::points_in_plan() const {
	size_t prod_points = production_points.size();
	size_t cons_points = consumption_points.size();
	size_t points = 0;
	for (size_t i = 0; i < prod_points; ++i) {
		for (size_t j = 0; j < cons_points; ++j) {
			if (plan[i][j] >= 0) {
				++points;
			}
		}
	}
	return points;
}

bool Potential::is_optimal_plan(size_t& i_, size_t& j_) const {
	std::vector<std::pair<size_t, size_t>> potential_pairs(points_in_plan());
	size_t prod_points = production_points.size();
	size_t cons_points = consumption_points.size();

	size_t it = 0;
	for (size_t i = 0; i < prod_points; ++i) {
		for (size_t j = 0; j < cons_points; ++j) {
			if (plan[i][j] >= 0) {
				potential_pairs[it++] = { j, i };
			}
		}
	}

	for (auto& pair : potential_pairs) {
		std::cout << "Pair: " << pair.first << " : " << pair.second << std::endl;
	}
	
	std::vector<bool> defined_prod_points(prod_points), defined_cons_points(cons_points);
	std::vector<int> u(prod_points), v(cons_points);
	std::queue<std::pair<bool, size_t>> queue;

	defined_prod_points[0] = true;
	queue.push({ true, 0 });

	while (!queue.empty()) {
		std::pair<bool, size_t> cur_var = queue.front();
		queue.pop();
		if (cur_var.first) {
			for (auto const& pair : potential_pairs) {
				if (!defined_cons_points[pair.first] && pair.second == cur_var.second) {
					defined_cons_points[pair.first] = true;
					v[pair.first] = table[pair.second][pair.first] + u[pair.second];
					queue.push({ false, pair.first });
				}
			}
		}
		else {
			for (auto const& pair : potential_pairs) {
				if (!defined_prod_points[pair.second] && pair.first == cur_var.second) {
					defined_cons_points[pair.second] = true;
					u[pair.second] = v[pair.first] - table[pair.second][pair.first];
					queue.push({ true, pair.second });
				}
			}
		}
	}

	std::cout << "u:\n";
	for (auto elem : u) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
	std::cout << "v:\n";
	for (auto elem : v) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	return is_optimal_plan(u, v, i_, j_);
}

bool Potential::is_optimal_plan(std::vector<int> const& u, std::vector<int> const& v, size_t& i_, size_t& j_) const {
	size_t prod_points = production_points.size();
	size_t cons_points = consumption_points.size();

	for (size_t i = 0; i < prod_points; ++i) {
		for (size_t j = 0; j < cons_points; ++j) {
			if (v[j] - u[i] > table[i][j]) {
				i_ = i;
				j_ = j;
				return false;
			}
		}
	}
	return true;
}

std::vector<int> Potential::solve() {

	northwest_corner_method();

	size_t i, j;
	while (!is_optimal_plan(i, j)) {
		std::cout << "Next: " << i << " : " << j << std::endl;
		break;
	}

	return std::vector<int>(1);
}

Potential::~Potential() {}