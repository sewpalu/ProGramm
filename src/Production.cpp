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

std::string Production::to_string()
{
  std::string output = "'" + this->lhs.getIdentifier() + "' ->";
  for (unsigned int i = 0; i < this->rhs.size(); i++)
  {
    output += " ' " + this->rhs.at(i)->getIdentifier() + "'";
  }
  return output;
}