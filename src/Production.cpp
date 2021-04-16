#include "Production.hpp"

Production::Production(Nonterminal lhs_input, std::vector<Symbol*> rhs_input)
{
  this->lhs = lhs_input;
  std::transform(
      rhs_input.begin(), rhs_input.end(), std::back_inserter(rhs),
      [](const auto& symbol) { return std::unique_ptr<Symbol>{symbol}; });
}

Production::~Production()
{
}
