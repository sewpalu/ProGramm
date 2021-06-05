#pragma once

#include <vector>

#include "Nonterminal.hpp"
#include "Symbol.hpp"

class Production
{
private:
  Nonterminal m_lhs;
  std::vector<std::unique_ptr<Symbol>> m_rhs;

public:
  Production(Nonterminal lhs_input, std::vector<Symbol*> rhs_input);
  Production(const Production& other);
  Production& operator=(const Production& other);
  ~Production();

  const Nonterminal& lhs() const;
  const std::vector<std::unique_ptr<Symbol>>& rhs() const;

  std::string to_string() const;

  bool is_cnf() const;
};
