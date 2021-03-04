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

		//LinearProgramming::Linear* linear = potential->get_linear();

		std::cout << std::endl;
		std::cout << "##############################################################################\n";
		std::cout << "##############################################################################\n";
		std::cout << "##############################################################################\n";

		/*std::vector<double> op = linear->solve_task();
		std::cout << "Optimal:\b";
		for (auto elem : op) {
			std::cout << elem << " ";
		}

		delete potential;*/
	}
	return 0;
}