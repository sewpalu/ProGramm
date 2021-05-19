#ifndef SIMPLE_WORD_PARSER_HPP
#define SIMPLE_WORD_PARSER_HPP

#include "GrammarParser.hpp"
#include <stdexcept>

class SimpleWordParser : public GrammarParser
{
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
    return {Nonterminal{""}, {}};
  }

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
