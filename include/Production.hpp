#pragma once

#include <vector>

#include "Symbol.h"
#include "Nonterminal.h"
class Production
{

public:
	Nonterminal lhs;
	std::vector<Symbol> rhs;

	Production(Nonterminal lhs_input, std::vector<Symbol> rhs_input);
	~Production();
};

