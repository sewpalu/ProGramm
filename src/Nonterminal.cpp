#include "Nonterminal.hpp"

Nonterminal::Nonterminal(std::string identifier_input, bool startSymbolInput)
{
	this->identifier = identifier_input;
	this->startSymbol = startSymbolInput;
}

bool Nonterminal::isStartSymbol() const
{
	return this->startSymbol;
}

void Nonterminal::setStartSymbol(bool startSymbolStatus)
{
	this->startSymbol = startSymbolStatus;
}
