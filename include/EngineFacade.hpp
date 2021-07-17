#ifndef ENGINE_FACADE_H
#define ENGINE_FACADE_H

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

#include "FormalGrammar.hpp"
#include "GrammarParser.hpp"
#include "SyntaxTree.hpp"
#include "WordParser.hpp"

/**
 * Facade class for interacting with the engine.
 *
 * Currently it doesn't serve much function, but should be used to simplify
 * working with
 * - the datamodel
 * - visualisation
 * - the various algorithms
 */
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

  /**
   * Copy assignment operator
   *
   * Copies GrammarParser according to its dynamic type
   */
  EngineFacade& operator=(const EngineFacade& other)
  {
    m_grammar = other.m_grammar;

    m_grammar_parser = other.m_grammar_parser->clone();

    return *this;
  }

  /**
   * Convenience function for setting the grammar.
   *
   * This should in the future not be done directly.
   */
  void setGrammar(const FormalGrammar& grammar)
  {
    m_grammar = grammar;
  }

  /**
   * Parses a word with the configured grammar
   */
  std::vector<SyntaxTree> parseWord(WordParser& word_parser,
                                    const std::string& word) const
  {
    if (m_grammar)
      return word_parser.parse(*m_grammar, m_grammar_parser->parseWord(word));
    return {};
  }
};

#endif /* ENGINE_FACADE_H */
