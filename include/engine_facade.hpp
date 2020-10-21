#ifndef ENGINE_FACADE_H
#define ENGINE_FACADE_H

#include <memory>
#include <utility>

#include "formal_grammar.hpp"
#include "grammar_parser.hpp"
#include "syntax_tree.hpp"
#include "word_parser.hpp"

class EngineFacade
{
private:
  std::optional<FormalGrammar> m_grammar;
  std::unique_ptr<GrammarParser> m_grammar_parser;

public:
  explicit EngineFacade(std::unique_ptr<GrammarParser> grammar_parser)
      : m_grammar_parser(std::move(grammar_parser))
  {
  }

  EngineFacade(const EngineFacade& other)
  {
    *this = other;
  }

  EngineFacade& operator=(const EngineFacade& other)
  {
    m_grammar = other.m_grammar;

    m_grammar_parser = other.m_grammar_parser->clone();

    return *this;
  }

  void setGrammarParser(std::unique_ptr<GrammarParser> grammar_parser)
  {
    m_grammar_parser = std::move(grammar_parser);
  }

  void addProduction(const std::string& production)
  {
    (void)production;
    throw std::runtime_error{"Parsing productions not implemented yet"};
  }

  std::vector<SyntaxTree> parseWord(std::unique_ptr<WordParser> word_parser,
                                    const std::string& word) const
  {
    if (m_grammar && word_parser)
      return word_parser->parse(*m_grammar, m_grammar_parser->parseWord(word));
    return {};
  }
};

#endif /* ENGINE_FACADE_H */
