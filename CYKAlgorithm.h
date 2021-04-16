#pragma once
#include "WordParser.h"

#include <vector>
#include <typeinfo>
#include <iostream>
#include <chrono>

#include "SyntaxTree.h"
#include "FormalGrammar.h"
#include "Word.h"
#include "CYKVisualiser.h"
#include "CYKLink.h"
#include "Nonterminal.h"
#include "SyntaxTree.h"
#include "STNode.h"

class CYKAlgorithm :
    public WordParser
{

public:
    std::pair<std::vector<SyntaxTree>, CYKVisualiser> parse(FormalGrammar grammar, Word input);

    CYKAlgorithm();
    ~CYKAlgorithm();
};

