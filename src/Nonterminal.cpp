#include "Nonterminal.hpp"

Nonterminal::Nonterminal(std::string identifier_input, bool startSymbolInput)
{
	this->identifier = identifier_input;
	this->startSymbol = startSymbolInput;
}

Nonterminal::Nonterminal(std::string identifier_input)
{
	this->identifier = identifier_input;
	this->startSymbol = false;
}

bool Nonterminal::isStartSymbol()
{
	return this->startSymbol;
}

void Nonterminal::setStartSymbol(bool startSymbolStatus)
{
	this->startSymbol = startSymbolStatus;
}
