#pragma once

#include <vector>

#include "Nonterminal.hpp"
#include "Symbol.hpp"

/**
 * Class representing a production rule.
 *
 * Consists of one nonterminal "lhs" that produces 0..n symbols "rhs"
 */
class Production
{
private:
  Nonterminal m_lhs;
  std::vector<std::unique_ptr<Symbol>> m_rhs;

public:
  Production(Nonterminal lhs_input, std::vector<Symbol*> rhs_input);

  /**
   * Copying is non-trivial, since the Symbols in m_rhs are dynamically typed.
   */
  Production(const Production& other);
  Production& operator=(const Production& other);

  const Nonterminal& lhs() const;
  const std::vector<std::unique_ptr<Symbol>>& rhs() const;

  std::string to_string() const;

  /**
   * Whether the production is allowed under the Chomsky normal form.
   */
  bool is_cnf() const;
};
