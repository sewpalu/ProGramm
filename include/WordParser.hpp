#pragma once

#include <memory>
#include <vector>

#include "SyntaxTree.hpp"
#include "Word.hpp"
#include "FormalGrammar.hpp"

class WordParser
{

public:
	virtual std::pair<std::vector<SyntaxTree>, std::shared_ptr<Visualiser>> parse(FormalGrammar grammar, Word input) = 0;

	WordParser();
	virtual ~WordParser();

};

