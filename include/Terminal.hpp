#pragma once
#include "Symbol.hpp"

#include <string>

struct Terminal :
    public Symbol
{

public:
    std::string value;

    Terminal(std::string identifier_input, std::string value_input);
    Terminal();
    ~Terminal();

    std::unique_ptr<Symbol> clone() override
    {
      return std::make_unique<Terminal>(*this);
    }
};

