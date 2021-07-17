#include "Symbol.hpp"

Symbol::Symbol(std::string identifier_input)
{
  this->identifier = identifier_input;
}

std::string Symbol::getIdentifier() const
{
  return this->identifier;
}
