#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

#include "symbol.hpp"

class Terminal : public Symbol
{
  const std::string m_value;

public:
  Terminal(const std::string& value, const std::string& identifier)
      : Symbol(identifier), m_value(value)
  {
  }

  std::unique_ptr<Symbol> clone() override
  {
    return std::make_unique<Terminal>(*this);
  }

  const std::string& value() const;
};

#endif /* TERMINAL_H */
