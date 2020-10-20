#ifndef FORMAL_GRAMMAR_H
#define FORMAL_GRAMMAR_H

#include <vector>

#include "nonterminal.hpp"
#include "production.hpp"

struct FormalGrammar
{
  std::vector<Production> productions;
  Nonterminal start;
};

#endif /* FORMAL_GRAMMAR_H */
