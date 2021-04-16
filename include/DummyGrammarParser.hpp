#ifndef DUMMY_GRAMMAR_PARSER_H
#define DUMMY_GRAMMAR_PARSER_H

#include "FormalGrammar.hpp"
#include "GrammarParser.hpp"

class DummyGrammarParser : public GrammarParser
{
private:
  static const FormalGrammar demo_grammar;
  static const Production demo_production;

public:
  std::unique_ptr<GrammarParser> clone() const override
  {
    return std::make_unique<DummyGrammarParser>(*this);
  }

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
