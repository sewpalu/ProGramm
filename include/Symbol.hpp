#pragma once

#include <string>
#include <memory>

#include <iostream>

class Symbol
{

public:
  std::string identifier;

  Symbol();
  ~Symbol();
  Symbol(std::string identifier_input);

  auto operator<=>(const Symbol& other) const = default;

  std::string getIdentifier() const;

  virtual std::unique_ptr<Symbol> clone() = 0;
};
