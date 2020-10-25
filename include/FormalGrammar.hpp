#pragma once

#include <vector>

#include "Production.hpp"
#include "Nonterminal.hpp"

struct FormalGrammar
{

public:
	std::vector<Production> rules;

	Nonterminal start;

	FormalGrammar(Nonterminal start_input, std::vector<Production> rules);
	~FormalGrammar();

};

