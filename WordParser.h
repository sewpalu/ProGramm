#pragma once

#include <vector>

#include "SyntaxTree.h"

class WordParser
{

public:
	std::vector<SyntaxTree> parse(); //FormalGrammar grammar, Word input

	WordParser();
	~WordParser();

};

