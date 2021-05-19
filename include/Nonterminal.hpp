#pragma once
#include "Symbol.hpp"

#include <string>

class Nonterminal :
    public Symbol
{

public:
    explicit Nonterminal(std::string identifier_input, bool startSymbolInput = false);
    Nonterminal() = delete;
    ~Nonterminal() = default;

    bool isStartSymbol();
    void setStartSymbol(bool startSymbolStatus);


    std::unique_ptr<Symbol> clone() override
    {
      return std::make_unique<Nonterminal>(*this);
    }

private:
    bool startSymbol;
};

