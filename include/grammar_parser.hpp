#ifndef GRAMMAR_PARSER_H
#define GRAMMAR_PARSER_H

#include <string>

#include "formal_grammar.hpp"
#include "production.hpp"
#include "word.hpp"

class GrammarParser
{
public:
  virtual ~GrammarParser() = default;

  virtual Production parseProduction(const std::string& input) const = 0;
  virtual FormalGrammar parseGrammar(const std::string& input) const = 0;
  virtual Word parseWord(const std::string& input) const = 0;

protected:
  GrammarParser() = default;
};

#endif /* GRAMMAR_PARSER_H */
