#include "Nonterminal.hpp"

Nonterminal::Nonterminal(std::string identifier_input, bool startSymbolInput)
    : Symbol(identifier_input), startSymbol(startSymbolInput)
{
}

bool Nonterminal::isStartSymbol() const
{
  return this->startSymbol;
}

void Nonterminal::setStartSymbol(bool startSymbolStatus)
{
  this->startSymbol = startSymbolStatus;
}
