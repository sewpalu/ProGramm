#pragma once
#include "Symbol.hpp"

#include <memory>
#include <string>

/**
 * Class representing a terminal.
 *
 * In addition to its identifier it has a value, which makes it possible to
 * check their identity independent of their values.
 * E.g. in a grammar, digits might have the identity "d" but the respective value might be "7" for instance.
 */
struct Terminal : public Symbol
{
  std::string value;

  Terminal(std::string identifier_input, std::string value_input);

  virtual ~Terminal() = default;

  /**
   * This allows copying the dynamic type
   */
  std::unique_ptr<Symbol> clone() override
  {
    return std::make_unique<Terminal>(*this);
  }
};
