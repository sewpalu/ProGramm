#pragma once
#include "Symbol.hpp"

#include <string>
#include <memory>

struct Terminal :
    public Symbol
{

public:
    std::string value;

    Terminal(std::string identifier_input, std::string value_input);
    Terminal() = delete;
    virtual ~Terminal() = default;

    std::unique_ptr<Symbol> clone() //override
    {
      return std::make_unique<Terminal>(*this);
    }
};

