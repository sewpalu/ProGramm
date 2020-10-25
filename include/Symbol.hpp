#pragma once

#include <string>
#include <memory>

struct Symbol
{

public:
	std::string identifier;

	Symbol();
	virtual ~Symbol();

	virtual std::unique_ptr<Symbol> clone() = 0;

	std::string getIdentifier();

protected:
	explicit Symbol(std::string identifier_input);


};

