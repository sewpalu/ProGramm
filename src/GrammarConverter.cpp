#include "GrammarConverter.hpp"


GrammarConverter::GrammarConverter()
{
}

FormalGrammar GrammarConverter::load_grammar_from_std_file()
{
  FormalGrammar test(Nonterminal("asdf", true), {});
  return test;
}

void GrammarConverter::save_grammar_to_std_file(FormalGrammar save_grammar)
{
  std::string file_name = "grammars.json";
}
