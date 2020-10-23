#pragma once

#include "Symbol.h"
#include "Nonterminal.h"
#include <vector>

class CYKLink
{

private:
	Nonterminal root;
	std::vector<std::pair<unsigned, unsigned int>> productions;

public:
	Nonterminal getRoot();
	std::vector<std::pair<unsigned, unsigned int>> getProductions();


	CYKLink(Nonterminal root_input, std::vector<std::pair<unsigned, unsigned int>> productions_input);
	CYKLink();
	~CYKLink();

	void setRoot(Nonterminal root_input);
	void setProductions(std::vector<std::pair<unsigned, unsigned int>> productions_input);
	void addProduction(std::pair<unsigned int, unsigned int> production_input);
	void emptyProductions();

	bool isEmpty();
};

