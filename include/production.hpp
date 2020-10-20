#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <memory>
#include <vector>

#include "nonterminal.hpp"
#include "symbol.hpp"

struct Production
{
  Nonterminal lhs;
  std::vector<std::unique_ptr<Symbol>> rhs;  ///< Using pointer for polymorphism

  // utility functions here
};

#endif /* PRODUCTION_H */
