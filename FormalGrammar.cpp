#include "FormalGrammar.h"

FormalGrammar::FormalGrammar()
{
}

FormalGrammar::FormalGrammar(Nonterminal start_input, std::vector<Production> rules_input)
{
	this->start = start_input;
	this->rules = rules_input;
}

FormalGrammar::~FormalGrammar()
{
}

std::vector<Symbol> FormalGrammar::getRHS(Nonterminal searchRoot)
{
	for (unsigned int ruleCounter = 0; ruleCounter < rules.size(); ruleCounter++)
	{
		if (rules.at(ruleCounter).lhs.getIdentifier() == searchRoot.getIdentifier())
		{
			return rules.at(ruleCounter).rhs;
		}
	}

	//If there is no RHS found, there can't be a rule with the given Nonterminal. Thus there is no rule in the grammar which contains the Nonterminal given as parameter to this function
	throw 0;
}

void FormalGrammar::addProduction(Production inputProduction)
{
	this->rules.push_back(inputProduction);
}
