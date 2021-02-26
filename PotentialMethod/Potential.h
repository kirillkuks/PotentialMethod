#pragma once
#include <vector>
#include <queue>
#include "Matrix.h"

struct PotentialParams {
	std::vector<int> production_points;
	std::vector<int> consumption_points;
	Matrix table;
};

// Класс метода потенциалов
class Potential {
public:
	Potential(PotentialParams const&);

	void northwest_corner_method();   // метод северо-западного угла
	std::vector<int> solve();   // решение задачи

	~Potential();
private:
	bool is_optimal_plan(size_t&, size_t&) const;   // проверка оптимальности точки
	bool is_optimal_plan(std::vector<int> const&, std::vector<int> const&, size_t&, size_t&) const;   // проверка оптимальности точки
	size_t points_in_plan() const;   // количество пар в плане


	std::vector<int> production_points;   // количество груза в пунктах хранения
	std::vector<int> consumption_points;   // потребность в пунктах назначения
	Matrix table;   // таблица стоимости перевозок
	Matrix plan;   // план перевозок
};