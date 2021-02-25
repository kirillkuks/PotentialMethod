#include "Parser.h"

Parser::Parser(std::string const& in) : ifs{ in } {}

Potential* Parser::parse() {
	std::string line;
	PotentialGrammar grammar;

	KW keyword = KW::KW_NULL;

	size_t production_number, consumption_number;
	std::vector<int> production_points, consumption_points;
	Matrix table;
	size_t cur_line = 0;
	while (std::getline(ifs, line)) {

		//std::cout << line << std::endl;
		if (line == grammar.get_keyword(KW::KW_SIZE)) {
			keyword = KW::KW_SIZE;
		}
		else if (line == grammar.get_keyword(KW::KW_PRODACTION)) {
			keyword = KW::KW_PRODACTION;
		}
		else if (line == grammar.get_keyword(KW::KW_CONSUMPTION)) {
			keyword = KW::KW_CONSUMPTION;
		}
		else if (line == grammar.get_keyword(KW::KW_TABLE)) {
			keyword = KW::KW_TABLE;
		}
		else {
			std::stringstream ss;
			ss << line;

			switch (keyword) {
			case KW::KW_SIZE:
				ss >> production_number;
				ss >> consumption_number;

				table = Matrix::create_matrix(production_number, consumption_number);
				production_points = create_vector(production_number);
				consumption_points = create_vector(consumption_number);
				break;
			case KW::KW_PRODACTION:
				for (auto& elem : production_points) {
					ss >> elem;
				}
				break;
			case KW::KW_CONSUMPTION:
				for (auto& elem : consumption_points) {
					ss >> elem;
				}
				break;
			case KW::KW_TABLE:
				for (size_t i = 0; i < consumption_number; ++i) {
					ss >> table[cur_line][i];
				}
				++cur_line;
				break;
			default:
				break;
			}
		}
	}

	PotentialParams pp{ production_points, consumption_points, table };

	return new Potential(pp);
}

std::vector<int> Parser::create_vector(size_t size) {
	return std::vector<int>(size);
}

Parser::~Parser() {
	ifs.close();
}

Grammar::Grammar(std::vector<std::string> keywords) : keywords{ keywords } {}

const std::string& Grammar::get_keyword(size_t index) const {
	return keywords[index];
}

size_t Grammar::size() const {
	return keywords.size();
}

PotentialGrammar::PotentialGrammar() : Grammar({ "Size", "Production", "Consumption", "Table" }) {}