#pragma once
#include "Symbol.h"

#include <string>

class Terminal :
    public Symbol
{

public:
    std::string value;

    Terminal(std::string identifier_input, std::string value_input);
    Terminal();
    ~Terminal();

};

