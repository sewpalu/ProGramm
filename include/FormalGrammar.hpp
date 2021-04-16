#pragma once

#include <vector>

#include "Production.h"
#include "Nonterminal.h"

class FormalGrammar
{

public:
	std::vector<Production> rules;

	Nonterminal start;

	FormalGrammar(Nonterminal start_input, std::vector<Production> rules);
	~FormalGrammar();

};

