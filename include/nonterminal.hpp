#ifndef NONTERMINAL_H
#define NONTERMINAL_H

#include "symbol.hpp"

class Nonterminal : public Symbol
{
public:
  explicit Nonterminal(const std::string& identifier) : Symbol(identifier)
  {
  }

  std::unique_ptr<Symbol> clone() override
  {
    return std::make_unique<Nonterminal>(*this);
  }
};

#endif /* NONTERMINAL_H */
