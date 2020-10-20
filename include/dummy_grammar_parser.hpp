#ifndef DUMMY_GRAMMAR_PARSER_H
#define DUMMY_GRAMMAR_PARSER_H

#include "formal_grammar.hpp"
#include "grammar_parser.hpp"

class DummyGrammarParser : GrammarParser
{
private:
  static const FormalGrammar demo_grammar;
  static const Production demo_production;

public:
  Production parseProduction(const std::string& input) const override
  {
    (void)input;
    return demo_production;
  }

  FormalGrammar parseGrammar(const std::string& input) const override
  {
    (void)input;
    return demo_grammar;
  }

  Word parseWord(const std::string& input) const override
  {
    auto word = Word{};
    for (const auto& character : input)
      word.content.push_back(Terminal{{character}, {}});

    return word;
  }
};

#endif /* DUMMY_GRAMMAR_PARSER_H */
