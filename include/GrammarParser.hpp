#ifndef GRAMMAR_PARSER_H
#define GRAMMAR_PARSER_H

#include <memory>
#include <string>

#include "FormalGrammar.hpp"
#include "Production.hpp"
#include "Word.hpp"

/**
 * Class for converting a string to a grammar.
 */
class GrammarParser
{
public:
  virtual ~GrammarParser() = default;

  /**
   * Needed for copying dynamic objects of subclasses.
   */
  virtual std::unique_ptr<GrammarParser> clone() const = 0;

  /**
   * Parses a production string (e.g. "S -> AB") into a Production object.
   */
  virtual Production parseProduction(const std::string& input) const = 0;

  /**
   * Parses the textual representation of a grammar into a FormalGrammar object.
   */
  virtual FormalGrammar parseGrammar(const std::string& input) const = 0;

  /**
   * Parses a word string into a word object
   */
  virtual Word parseWord(const std::string& input) const = 0;
};

#endif /* GRAMMAR_PARSER_H */
