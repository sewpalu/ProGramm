#include "GrammarConverter.hpp"


GrammarConverter::GrammarConverter()
{
  std::cout << "Constructor\n";
  nlohmann::json test;

}


GrammarConverter::~GrammarConverter()
{
}

FormalGrammar GrammarConverter::load_grammar_from_std_file()
{
  nlohmann::json loaded_data;
  FormalGrammar test(Nonterminal("asdf", true), {});
  return test;
}

void GrammarConverter::save_grammar_to_std_file(FormalGrammar save_grammar)
{
  nlohmann::json output_data;
  std::string file_name = "grammars.json";
}
