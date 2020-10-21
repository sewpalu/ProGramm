#pragma once
#include "WordParser.h"

#include <vector>

#include "SyntaxTree.h"

class CYKAlgorithm :
    public WordParser
{

public:
    std::vector<SyntaxTree> parse(); //FormalGrammar grammar, Word input

    CYKAlgorithm();
    ~CYKAlgorithm();
};

