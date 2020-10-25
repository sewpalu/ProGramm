#include "Symbol.hpp"

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

Symbol::Symbol(std::string identifier_input)
{
	this->identifier = identifier_input;
}

std::string Symbol::getIdentifier()
{
	return this->identifier;
}
