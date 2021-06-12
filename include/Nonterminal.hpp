#pragma once
#include "Symbol.hpp"

#include <string>

#include <iostream>

class Nonterminal :
    public Symbol
{

public:
    explicit Nonterminal(std::string identifier_input, bool startSymbolInput = false);
    Nonterminal() = delete;
    virtual ~Nonterminal() = default;

    bool isStartSymbol() const;
    void setStartSymbol(bool startSymbolStatus);


    std::unique_ptr<Symbol> clone() override
    {
      //std::cout << "Cloning: " << this->identifier << "\n";
      return std::make_unique<Nonterminal>(*this);
    }

private:
    bool startSymbol;
};

