#ifndef NONTERMINAL_H
#define NONTERMINAL_H

#include "symbol.hpp"

class Nonterminal : public Symbol
{
public:
  explicit Nonterminal(const std::string& identifier);
};

#endif /* NONTERMINAL_H */
