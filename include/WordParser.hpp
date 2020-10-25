#pragma once

#include <algorithm>
#include <vector>

#include "SyntaxTree.hpp"
#include "FormalGrammar.hpp"
#include "Word.hpp"

class WordParser
{

public:
	virtual std::unique_ptr<Visualiser> parse(FormalGrammar grammar, Word input) = 0;
	WordParser();
	virtual ~WordParser();

};

