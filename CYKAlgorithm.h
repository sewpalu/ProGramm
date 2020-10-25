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

class CYKAlgorithm :
    public WordParser
{

public:
    CYKVisualiser parse(FormalGrammar grammar, Word input); //FormalGrammar grammar, Word input

    CYKAlgorithm();
    ~CYKAlgorithm();
};

