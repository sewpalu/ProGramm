#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include "Nonterminal.hpp"
#include "Symbol.hpp"
#include "FormalGrammar.hpp"

class GrammarConverter
{

public:
  GrammarConverter();
  ~GrammarConverter()

  FormalGrammar load_grammar_from_std_file();

  //FormalGrammar load_grammar_from_file_path(std::string file_path);

  void save_grammar_to_std_file(FormalGrammar save_grammar);

  //void save_gramar_to_file_path(std::string file_path,
  //                              FormalGrammar save_grammar);
};
