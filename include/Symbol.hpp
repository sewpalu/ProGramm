#pragma once

#include <string>
#include <memory>

class Symbol
{

public:
  std::string identifier;

  Symbol();
  ~Symbol();
  Symbol(std::string identifier_input);

  std::string getIdentifier();

  virtual std::unique_ptr<Symbol> clone() = 0;
};
