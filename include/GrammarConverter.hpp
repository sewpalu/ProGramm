#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>
#include <string>

#include "nlohmann/json.hpp"
#include "wx/string.h"

#include "FormalGrammar.hpp"
#include "Nonterminal.hpp"
#include "Symbol.hpp"
#include "Terminal.hpp"

class GrammarConverter
{
public:
  struct GrammarStruct
  {
    Nonterminal start_symbol = Nonterminal{""};
    std::vector<Nonterminal*> nonterminals;
    std::vector<Terminal*> terminals;
    std::vector<Production> productions;
  };

public:
  GrammarConverter() = default;
  ~GrammarConverter() = default;

  GrammarStruct load_grammar_from_std_file(std::string grammar_name);

  // FormalGrammar load_grammar_from_file_path(std::string file_path);

  void save_grammar_to_std_file(std::vector<Nonterminal*> nonterminal_alphabet,
                                std::vector<Terminal*> terminal_alphabet,
                                std::vector<Production> productions,
                                std::string grammar_name);

  // void save_gramar_to_file_path(std::string file_path,
  //                              FormalGrammar save_grammar);

  bool grammar_exists(std::string grammar_name);

  std::vector<wxString> get_grammar_names();

  bool delete_grammar(std::string grammar_name);

  std::vector<Nonterminal*> get_nonterminal_alphabet_from_grammar(
      FormalGrammar grammar);
  std::vector<Terminal*> get_terminal_alphabet_from_grammar(
      FormalGrammar grammar);

  bool symbol_in_vector(std::vector<Symbol*> symbols, std::string identifier);

  bool terminal_in_vector(std::vector<Terminal*> terminals,
                          std::string identifier);
  bool nonterminal_in_vector(std::vector<Nonterminal*> nonterminals,
                             std::string identifier);

private:
  std::string grammar_file_name = "grammars.json";
};
