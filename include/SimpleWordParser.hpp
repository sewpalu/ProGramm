#ifndef SIMPLE_WORD_PARSER_HPP
#define SIMPLE_WORD_PARSER_HPP

#include "GrammarParser.hpp"
#include <stdexcept>

/**
 * GrammarParser that can't parse grammars but only words.
 */
class SimpleWordParser : public GrammarParser
{
public:
  /**
   * Allows copying the dynamic type.
   */
  virtual std::unique_ptr<GrammarParser> clone() const override
  {
    return std::make_unique<SimpleWordParser>(*this);
  }

  virtual Production parseProduction(const std::string&) const override
  {
    throw std::runtime_error{
        "SimpleWordParser doesn't implement parseProduction()"};
    return {Nonterminal{""}, {}};
  }

  virtual FormalGrammar parseGrammar(const std::string&) const override
  {
    throw std::runtime_error{
        "SimpleWordParser doesn't implement parseGrammar()"};
    return {.rules = {}, .start = Nonterminal{""}};
  }

  /**
   * Converts a string to a Word.
   *
   * Each character in the string is assumed to be a single Terminal.
   */
  virtual Word parseWord(const std::string& input) const final
  {
    auto word = Word{};

    // FIXME: This doesn't support Unicode (only ASCII)
    for (auto c : input)
      word.content.push_back(Terminal{{c}, {c}});

    return word;
  }
};

#endif /* ifndef SIMPLE_WORD_PARSER_HPP */
