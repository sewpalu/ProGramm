#pragma once
#include "Symbol.hpp"

#include <string>

#include <iostream>

/**
 * Class representing a nonterminal
 *
 * For convenience the class may also know, whether it is a start symbol.
 */
class Nonterminal :
    public Symbol
{

public:
    explicit Nonterminal(std::string identifier_input, bool startSymbolInput = false);
    Nonterminal() = delete;
    virtual ~Nonterminal() = default;

    auto operator<=>(const Nonterminal& other) const = default;

    bool isStartSymbol() const;
    void setStartSymbol(bool startSymbolStatus);


    /**
     * Allows copying of the dynamic type
     */
    std::unique_ptr<Symbol> clone() override
    {
      return std::make_unique<Nonterminal>(*this);
    }

private:
    bool startSymbol;
};

