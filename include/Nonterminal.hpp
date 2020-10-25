#pragma once
#include "Symbol.hpp"

#include <string>

struct Nonterminal :
    public Symbol
{

public:
    explicit Nonterminal(std::string identifier_input);
    Nonterminal();
    ~Nonterminal();

    std::unique_ptr<Symbol> clone() override
    {
      return std::make_unique<Nonterminal>(*this);
    }
};

