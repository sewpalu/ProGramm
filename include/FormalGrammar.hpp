#ifndef FORMAL_GRAMMAR_H
#define FORMAL_GRAMMAR_H

#include <vector>

#include "Nonterminal.hpp"
#include "Production.hpp"

struct FormalGrammar
{
  std::vector<Production> productions;
  Nonterminal start;

  FormalGrammar(std::vector<Production> productions, Nonterminal start)
      : productions(productions), start(start)
  {
  }
};

#endif /* FORMAL_GRAMMAR_H */
