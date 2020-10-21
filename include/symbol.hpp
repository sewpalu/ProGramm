#ifndef SYMBOL_H
#define SYMBOL_H

#include <memory>
#include <string>

class Symbol
{
  std::string m_identifier;

public:
  virtual ~Symbol() = default;

  virtual std::unique_ptr<Symbol> clone() = 0;

  const std::string& identifier() const
  {
    return m_identifier;
  }

protected:
  explicit Symbol(const std::string& identifier) : m_identifier(identifier)
  {
  }
};

#endif /* SYMBOL_H */
