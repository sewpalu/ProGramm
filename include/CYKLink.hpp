#pragma once

#include "Symbol.hpp"
#include "Nonterminal.hpp"
#include <vector>

class CYKLink
{

private:
	Nonterminal root;
	//This vector will hold either two values or one value, depending on if the right hand side of the rule has two Nonterminals or one Terminal
	std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>> productions;
	//bool usedForProduction;

public:
	Nonterminal getRoot() const;
	std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>> getProductions() const;

	//void setUsed();

	//bool wasUsed();

	CYKLink(Nonterminal root_input, std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>> productions_input);
	CYKLink();
	CYKLink(Nonterminal root_input);
	~CYKLink();

	void setRoot(Nonterminal root_input);
	void setProductions(std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>> productions_input);
	void addProduction(std::pair<std::pair<unsigned int, unsigned int>, CYKLink> production_input);
	void emptyProductions();

	bool isEmpty();
};
