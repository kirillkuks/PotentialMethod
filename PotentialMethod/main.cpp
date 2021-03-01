#include "Potential.h"
#include "Parser.h"


int main(int argc, char* argv[]) {
	if (argc > 1) {
		Parser parser(argv[1]);
		Potential* potential = parser.parse();
		std::vector<int> optimal = potential->solve();

		std::cout << "\nOptimal:\n";
		for (auto elem : optimal) {
			std::cout << elem << " ";
		}
		std::cout << std::endl;
		std::cout << "Min value: " << potential->objective_function(optimal) << std::endl;

		delete potential;
	}
	return 0;
}