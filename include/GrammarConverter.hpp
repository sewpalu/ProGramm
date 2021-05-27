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

#include <wx/msgdlg.h>

class GrammarConverter
{

public:
  GrammarConverter(wxWindow* msg_dialog_parent);
  ~GrammarConverter();

  FormalGrammar load_grammar_from_std_file(std::string grammar_name);

  //FormalGrammar load_grammar_from_file_path(std::string file_path);

  void save_grammar_to_std_file(std::vector<Nonterminal*> nonterminal_alphabet, std::vector<Terminal*> terminal_alphabet, std::vector<Production> productions, std::string grammar_name);

  //void save_gramar_to_file_path(std::string file_path,
  //                              FormalGrammar save_grammar);

  bool grammar_exists(std::string grammar_name);

  std::vector<wxString> get_grammar_names();

  bool delete_grammar(std::string grammar_name);

  std::vector<Nonterminal*> get_nonterminal_alphabet_from_grammar(
      FormalGrammar grammar);
  std::vector<Terminal*> get_terminal_alphabet_from_grammar(
      FormalGrammar grammar);
  std::vector<Production> get_productions_from_grammar(FormalGrammar grammar);
  std::string get_grammar_name_from_grammar(FormalGrammar grammar);

private:
  std::string grammar_file_name = "grammars.json";
};
