#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iterator>
#include <memory>
#include <vector>

#include "Nonterminal.hpp"
#include "Symbol.hpp"

struct Production
{
  Nonterminal lhs;
  std::vector<std::unique_ptr<Symbol>> rhs;  ///< Using pointer for polymorphism

  Production(const Nonterminal& lhs, std::vector<std::unique_ptr<Symbol>>&& rhs)
      : lhs(lhs), rhs(std::move(rhs))
  {
  }

  Production(const Nonterminal& lhs,
             const std::vector<std::unique_ptr<Symbol>>& rhs)
      : lhs(lhs)
  {
    std::transform(rhs.begin(), rhs.end(), std::back_inserter(this->rhs),
                   [](const auto& symbol) { return symbol->clone(); });
  }

  Production(const Production& other) : lhs(other.lhs)
  {
    *this = other;
  }

  Production& operator=(const Production& other)
  {
    lhs = other.lhs;
    std::transform(other.rhs.begin(), other.rhs.end(), std::back_inserter(rhs),
                   [](const auto& symbol) { return symbol->clone(); });
    return *this;
  }
  // utility functions here
};

#endif /* PRODUCTION_H */
