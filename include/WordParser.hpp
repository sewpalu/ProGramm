#pragma once

#include <memory>
#include <vector>

#include "FormalGrammar.hpp"
#include "SyntaxTree.hpp"
#include "Visualisable.hpp"
#include "Word.hpp"

/**
 * Base class for classes parsing a word, given a formal grammar.
 *
 * Used for the strategy pattern.
 */
class WordParser : public Visualisable
{

public:
  virtual std::vector<SyntaxTree> parse(
      FormalGrammar grammar, Word input) = 0;

  WordParser(std::unique_ptr<Visualiser> visualiser);
  virtual ~WordParser();
};
