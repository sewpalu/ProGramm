#pragma once
#include "Symbol.h"

#include <string>

class Terminal :
    public Symbol
{

public:
    std::string value;

    Terminal(std::string identifier_input, std::string value_input);

    //Set identifier and value to same string
    Terminal(std::string content);
    Terminal();
    ~Terminal();

};

