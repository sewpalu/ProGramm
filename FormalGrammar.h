#pragma once

#include <vector>
#include <exception>

#include "Production.h"
#include "Nonterminal.h"

class FormalGrammar
{

public:
	std::vector<Production> rules;

	Nonterminal start;

	FormalGrammar();
	FormalGrammar(Nonterminal start_input, std::vector<Production> rules);
	~FormalGrammar();

	std::vector<Symbol> getRHS(Nonterminal searchRoot);

	void addProduction(Production inputProduction);

};

