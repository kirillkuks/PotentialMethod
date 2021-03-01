#include "Potential.h"

Potential::Potential(PotentialParams const& pp)
	: production_points{ pp.production_points },
	consumption_points{ pp.consumption_points },
	table{ pp.table },
	fic{ pp.fic } {
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

int Potential::objective_function(std::vector<int> const& x) const {
	size_t n_size = table.get_n();
	size_t m_size = table.get_m();

	int sum = 0;
	for (size_t i = 0, k = 0; i < n_size; ++i) {
		for (size_t j = 0; j < m_size; ++j) {
			sum += table[i][j] * x[k++];
		}
	}

	return sum;
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
					defined_prod_points[pair.second] = true;
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

	int delta = 0;

	for (size_t i = 0; i < prod_points; ++i) {
		for (size_t j = 0; j < cons_points; ++j) {
			if (v[j] - u[i] - table[i][j] > delta) {
				i_ = i;
				j_ = j;
				delta = v[j] - u[i] - table[i][j];
			}
		}
	}

	if (delta) {
		std::cout << "Delta: " << delta << std::endl;
		return false;
	}

	return true;
}

bool Potential::is_consumption_in_cycle(std::vector<size_t> const& vertexes, size_t cons) const {
	for (size_t j = 1; j < vertexes.size(); j += 2) {
		if (vertexes[j] == cons) {
			return true;
		}
	}
	return false;
}

bool Potential::is_production_in_cycle(std::vector<size_t> const& vertexes, size_t prod) const {
	for (size_t i = 2; i < vertexes.size(); i += 2) {
		if (vertexes[i] == prod) {
			return true;
		}
	}
	return false;
}

void Potential::build_cycle(std::vector<size_t>& vertexes) {
	size_t size = vertexes.size();

	if (size % 2) {
		for (size_t j = 0; j < plan.get_m(); ++j) {
			if (vertexes[size - 1] == vertexes[0] && j == vertexes[1]) {
				if (vertexes.size() > 3) {
					cycle = vertexes;
				}
				std::cout << "Size: " << vertexes.size() << std::endl;
			}
			if (plan[vertexes[size - 1]][j] > -1 && !is_consumption_in_cycle(vertexes, j)) {
				vertexes.push_back(j);
				build_cycle(vertexes);
				vertexes.pop_back();
			}
		}
	}
	else {
		for (size_t i = 0; i < plan.get_n(); ++i) {
			if (i == vertexes[0] && vertexes[size - 1] == vertexes[1]) {
				if (vertexes.size() > 3) {
					cycle = vertexes;
				}
			}
			if (plan[i][vertexes[size - 1]] > -1 && !is_production_in_cycle(vertexes, i)) {
				vertexes.push_back(i);
				build_cycle(vertexes);
				vertexes.pop_back();
			}
		}
	}
}

void Potential::build_cycle(size_t i, size_t j) {
	std::vector<size_t> vertexes(2);
	vertexes[0] = i;
	vertexes[1] = j;
	cycle.clear();

	build_cycle(vertexes);

	std::cout << "Vertexes:\n";
	for (auto vertex : cycle) {
		std::cout << vertex << " ";
	}

	return;
}

void Potential::recount() {
	size_t size = cycle.size();

	std::vector<std::pair<size_t, size_t>> chain(size - 1);

	for (size_t i = 0; i < size - 2; i += 2) {
		chain[i] = { cycle[i], cycle[i + 1] };
		chain[i + 1] = { cycle[i + 2], cycle[i + 1] };
	}

	std::cout << "\nCycle:\n";
	for (auto& pair : chain) {
		std::cout << "<" << pair.first << ", " << pair.second << ">; ";
	}
	std::cout << std::endl;

	int min = plan[chain[1].first][chain[1].second];
	for (size_t i = 3; i < size - 1; i += 2) {
		if (min > plan[chain[i].first][chain[i].second]) {
			min = plan[chain[i].first][chain[i].second];
		}
	}

	plan[chain[0].first][chain[0].second] = 0;
	for (size_t i = 0; i < size - 1; i += 2) {
		plan[chain[i].first][chain[i].second] += min;
	}
	bool closed = false;
	for (size_t i = 1; i < size - 1; i += 2) {
		plan[chain[i].first][chain[i].second] -= min;
		if (plan[chain[i].first][chain[i].second] == 0 && !closed) {
			plan[chain[i].first][chain[i].second] = -1;
			closed = true;
		}
	}

	std::cout << "\nMin: " << min << std::endl;
}

std::vector<int> Potential::get_plan() {
	size_t n_size = plan.get_n();
	size_t m_size = plan.get_m();
	std::vector<int> optimal(n_size * m_size);

	size_t it = 0;
	for (size_t i = 0; i < n_size; ++i) {
		for(size_t j = 0; j < m_size; ++j) {
			optimal[it++] = plan[i][j] > -1 ? plan[i][j] : 0;
		}
	}

	return optimal;
}

std::vector<int> Potential::solve() {

	northwest_corner_method();

	size_t i, j;
	while (!is_optimal_plan(i, j)) {
		std::cout << "Next: " << i << " : " << j << std::endl;

		build_cycle(i, j);
		recount();

		std::cout << "New plan:\n";
		plan.print();
	}

	std::cout << "Final plan:\n";
	plan.print();

	return get_plan();
}

Potential::~Potential() {}