#pragma once

#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>

#include <nlohmann/json.hpp>

#include "Nonterminal.hpp"
#include "Terminal.hpp"
#include "Symbol.hpp"
#include "FormalGrammar.hpp"

class GrammarConverter
{

public:
  GrammarConverter();
  ~GrammarConverter();

  FormalGrammar load_grammar_from_std_file();

  //FormalGrammar load_grammar_from_file_path(std::string file_path);

  void save_grammar_to_std_file(std::vector<Nonterminal*> nonterminal_alphabet, std::vector<Terminal*> terminal_alphabet, std::vector<Production> productions, std::string grammar_name);

  //void save_gramar_to_file_path(std::string file_path,
  //                              FormalGrammar save_grammar);
};
