#include "FormalGrammar.hpp"

FormalGrammar::FormalGrammar(Nonterminal start_input, std::vector<Production> rules_input)
{
	this->start = start_input;
	this->rules = rules_input;
}

FormalGrammar::~FormalGrammar()
{
}
