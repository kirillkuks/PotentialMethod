#pragma once
#include <vector>
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

	~Potential();
private:
	
	std::vector<int> production_points;   // количество груза в пунктах хранения
	std::vector<int> consumption_points;   // потребность в пунктах назначения
	Matrix table;   // таблица стоимости перевозок
	Matrix plan;   // план перевозок
};