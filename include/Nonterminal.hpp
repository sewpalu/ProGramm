#pragma once
#include "Symbol.h"

#include <string>

class Nonterminal :
    public Symbol
{

public:
    Nonterminal(std::string identifier_input);
    Nonterminal();
    ~Nonterminal();

};

