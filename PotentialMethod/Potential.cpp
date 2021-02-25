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

void Potential::northwest_coener_method() {
	Matrix plan(production_points.size(), consumption_points.size());
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

Potential::~Potential() {}