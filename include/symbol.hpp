#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

class Symbol
{
  const std::string m_identifier;

public:
  virtual ~Symbol() = default;

  const std::string& identifier() const
  {
    return m_identifier;
  }

protected:
  Symbol(const std::string& identifier) : m_identifier(identifier)
  {
  }
};

#endif /* SYMBOL_H */
