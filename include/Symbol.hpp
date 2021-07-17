#pragma once

#include <string>
#include <memory>

#include <iostream>

/**
 * Class representing a symbol, i.e. a terminal or non-terminal.
 *
 * Is identified by its name (member `identifier`).
 *
 * This is a struct on the grounds of C++ Core Guideline C.2
 * See: http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-struct
 */
struct Symbol
{
  std::string identifier = "";

  explicit Symbol(const std::string& identifier_input);
  virtual ~Symbol() = default;

  auto operator<=>(const Symbol& other) const = default;

  virtual std::unique_ptr<Symbol> clone() = 0;
};
