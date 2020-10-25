#include "Production.hpp"
#include <memory>

Production::Production(Nonterminal lhs_input, std::vector<std::unique_ptr<Symbol>> rhs_input)
{
	this->lhs = lhs_input;
	std::transform(rhs_input.begin(), rhs_input.end(), std::back_inserter(rhs), [](const auto& symbol) { return symbol->clone(); });
}

Production::~Production()
{
}
