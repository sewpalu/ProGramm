#pragma once
#include "WordParser.hpp"

#include <memory>
#include <vector>
#include <typeinfo>
#include <iostream>
#include <chrono>

#include "SyntaxTree.hpp"
#include "FormalGrammar.hpp"
#include "Word.hpp"
#include "CYKVisualiser.hpp"
#include "CYKLink.hpp"
#include "Nonterminal.hpp"
#include "SyntaxTree.hpp"

class CYKAlgorithm :
    public WordParser
{

public:
  std::unique_ptr<Visualiser> parse(FormalGrammar grammar, Word input) override;

    CYKAlgorithm();
    ~CYKAlgorithm();
};

