#include "Production.hpp"

#include <algorithm>
#include <iterator>

#include "Terminal.hpp"

Production::Production(Nonterminal lhs_input, std::vector<Symbol*> rhs_input)
    : m_lhs(lhs_input)
{
  std::transform(
      rhs_input.begin(), rhs_input.end(), std::back_inserter(m_rhs),
      [](const auto& symbol) { return std::unique_ptr<Symbol>{symbol}; });
}

Production::Production(const Production& other) : m_lhs(other.lhs())
{
  *this = other;
}

Production& Production::operator=(const Production& other)
{
  m_lhs = other.lhs();
  m_rhs.clear();
  std::transform(other.rhs().begin(), other.rhs().end(),
                 std::back_inserter(m_rhs),
                 [](const auto& symbol) { return symbol->clone(); });
  return *this;
}

Production::~Production()
{
}

const Nonterminal& Production::lhs() const
{
  return m_lhs;
}

const std::vector<std::unique_ptr<Symbol>>& Production::rhs() const
{
  return m_rhs;
}

bool Production::is_cnf() const
{
  return
      // S → ε
      (m_rhs.empty() && m_lhs.isStartSymbol()) ||

      // A → BC
      (m_rhs.size() == 2 &&
       std::all_of(m_rhs.begin(), m_rhs.end(),
                   [](const auto& symbol) {
                     return dynamic_cast<Nonterminal*>(symbol.get());
                   })) ||

      // A → a
      (m_rhs.size() == 1 && dynamic_cast<Terminal*>(m_rhs.front().get()));
}

std::string Production::to_string() const
{
  auto result = m_lhs.identifier + " → ";
  for (const auto& symbol : m_rhs)
  {
    result += symbol->getIdentifier();
  }
  return result;
}
