#pragma once
#include <vector>
#include <queue>
#include "Matrix.h"

enum Fictitious {
	FIC_NAN = 0,
	FIC_PRODUCTION,
	FIC_CONSUMPTION
};

struct PotentialParams {
	std::vector<int> production_points;
	std::vector<int> consumption_points;
	Matrix table;
	Fictitious fic;
};

// Класс метода потенциалов
class Potential {
public:
	Potential(PotentialParams const&);

	void northwest_corner_method();   // метод северо-западного угла
	std::vector<int> solve();   // решение задачи

	int objective_function(std::vector<int> const&) const;

	~Potential();
private:
	bool is_optimal_plan(size_t&, size_t&) const;   // проверка оптимальности точки
	bool is_optimal_plan(std::vector<int> const&, std::vector<int> const&, size_t&, size_t&) const;   // проверка оптимальности точки
	size_t points_in_plan() const;   // количество пар в плане

	void build_cycle(size_t, size_t);
	void recount();

	void build_cycle(std::vector<size_t>&);
	bool is_production_in_cycle(std::vector<size_t> const&, size_t) const;
	bool is_consumption_in_cycle(std::vector<size_t> const&, size_t) const;

	std::vector<int> get_plan() const;
	
	void take_away_fic(size_t&, size_t&) const;

	std::vector<size_t> cycle;   // цикл пересчёта
	std::vector<int> production_points;   // количество груза в пунктах хранения
	std::vector<int> consumption_points;   // потребность в пунктах назначения
	Matrix const table;   // таблица стоимости перевозок
	Matrix plan;   // план перевозок
	Fictitious fic;
};