#ifndef GRAMMAR_PARSER_H
#define GRAMMAR_PARSER_H

#include <memory>
#include <string>

#include "FormalGrammar.hpp"
#include "Production.hpp"
#include "Word.hpp"

class GrammarParser
{
public:
  virtual ~GrammarParser() = default;

  virtual std::unique_ptr<GrammarParser> clone() const = 0;

  virtual Production parseProduction(const std::string& input) const = 0;
  virtual FormalGrammar parseGrammar(const std::string& input) const = 0;
  virtual Word parseWord(const std::string& input) const = 0;

protected:
  GrammarParser() = default;
};

#endif /* GRAMMAR_PARSER_H */
