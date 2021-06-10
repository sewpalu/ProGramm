#pragma once

#include <memory>
#include <vector>

#include "FormalGrammar.hpp"
#include "SyntaxTree.hpp"
#include "Visualisable.hpp"
#include "Word.hpp"

class WordParser : public Visualisable
{

public:
  virtual std::vector<SyntaxTree> parse(
      FormalGrammar grammar, Word input) = 0;

  WordParser(std::unique_ptr<Visualiser> visualiser);
  virtual ~WordParser();
};
