#pragma once

#include <string>

class Symbol
{

public:
	std::string identifier;

	Symbol();
	~Symbol();
	Symbol(std::string identifier_input);

	std::string getIdentifier();

};

