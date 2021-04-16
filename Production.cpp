#include "Production.h"

Production::Production()
{
}

Production::Production(Nonterminal lhs_input, std::vector<Symbol> rhs_input)
{
	this->lhs = lhs_input;
	this->rhs = rhs_input;
}

Production::~Production()
{
}
