#pragma once

#include <vector>
#include <memory>

#include "Symbol.hpp"
#include "Nonterminal.hpp"
struct Production
{

public:
	Nonterminal lhs;
	std::vector<std::unique_ptr<Symbol>> rhs;

	Production(Nonterminal lhs_input, std::vector<Symbol*> rhs_input);
	~Production();

	Production(const Production& other) : lhs(other.lhs)
	{
	  *this = other;
	}
	
	Production& operator=(const Production& other)
	{
	  lhs = other.lhs;
          rhs.clear();
	  std::transform(other.rhs.begin(), other.rhs.end(), std::back_inserter(rhs),
	                 [](const auto& symbol) { return symbol->clone(); });
	  return *this;
	}
};

