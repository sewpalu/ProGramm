#include "Symbol.hpp"

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

Symbol::Symbol(std::string identifier_input)
{
  //std::cout << "Getting identifier\n";
	this->identifier = identifier_input;
}

std::string Symbol::getIdentifier() const
{
	return this->identifier;
}
