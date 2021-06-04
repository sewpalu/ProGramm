#include "GrammarConverter.hpp"


GrammarConverter::GrammarConverter(wxWindow* msg_dialog_parent)
{
  std::cout << "Constructor\n";
}


GrammarConverter::~GrammarConverter()
{
}

GRAMMAR_STRUCT GrammarConverter::load_grammar_from_std_file(
    std::string grammar_name)
{
  auto output_data = nlohmann::json::array();

  std::ifstream input_file;

  auto grammars_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->grammar_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> grammars_data;

      input_file.close();
    }
    else
    {
      std::cout << "Not open!\n";
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->grammar_file_name << "\n";
  }

  std::cout << "Loading grammar: " << grammar_name << "\n";

  GRAMMAR_STRUCT output_grammar;

  for (unsigned int grammar_number = 0; grammar_number < grammars_data.size();
       grammar_number++)
  {
    if (grammars_data.at(grammar_number)["name"] == grammar_name)
    {
      std::cout << "Grammar found\n";
      try
      {
        if (!(grammars_data.at(grammar_number)["startsymbol"] == ""))
        {
          output_grammar.start_symbol =
              Nonterminal(grammars_data.at(grammar_number)["startsymbol"]);
        }
      }
      catch (...)
      {
        //No start symbol is set yet, which is not a problem
      }

      std::vector<Terminal*> terminals;

      for (unsigned int terminal_index = 0;
           terminal_index < grammars_data.at(grammar_number)["alphabet"]["terminals"].size();
           terminal_index++)
      {
        std::string terminal_identifier =
            grammars_data.at(grammar_number)["alphabet"]["terminals"].at(terminal_index);
        terminals.push_back(new Terminal(terminal_identifier, terminal_identifier));
      }

      output_grammar.terminals = terminals;

      std::vector<Nonterminal*> nonterminals;

      for (unsigned int nonterminal_index = 0;
           nonterminal_index <
           grammars_data.at(grammar_number)["alphabet"]["nonterminals"].size();
           nonterminal_index++)
      {
        std::string terminal_identifier =
            grammars_data.at(grammar_number)["alphabet"]["nonterminals"].at(
                nonterminal_index);
        nonterminals.push_back(
            new Nonterminal(terminal_identifier));
      }

      output_grammar.nonterminals = nonterminals;

      std::vector<Production> productions;

      for (unsigned int production_index = 0;
           production_index <
           grammars_data.at(grammar_number)["rules"].size();
           production_index++)
      {
        Nonterminal lhs_symbol(grammars_data.at(grammar_number)["rules"].at(
            production_index)["LHS"]);

        std::vector<Symbol*> rhs_symbols;

        for (unsigned int rhs_index = 0;
             rhs_index <
             grammars_data.at(grammar_number)["rules"].at(production_index)["RHS"].size();
             rhs_index++)
        {
          std::string symbol_identifier = grammars_data.at(grammar_number)["rules"]
                                    .at(production_index)["RHS"]
                                    .at(rhs_index);

          if (this->nonterminal_in_vector(output_grammar.nonterminals,
                                     symbol_identifier))
          {
            rhs_symbols.push_back(new Nonterminal(symbol_identifier));
          }
          else if (this->terminal_in_vector(output_grammar.terminals, symbol_identifier))
          {
            rhs_symbols.push_back(
                new Terminal(symbol_identifier, symbol_identifier));
          }
          else
          {
            std::cout << "The grammar contains a symbol that is not part of "
                         "its alphabet: "
                      << symbol_identifier << "\n";
            std::cout << "This leads to incomplete loaded rules\n";
          }
        }
        productions.push_back(Production(lhs_symbol, rhs_symbols));
      }
      output_grammar.productions = productions;
    }
  }


  return output_grammar;
}

void GrammarConverter::save_grammar_to_std_file(std::vector<Nonterminal*> nonterminal_alphabet,
                                                std::vector<Terminal*> terminal_alphabet,
                                                std::vector<Production> productions,
                                                std::string grammar_name)
{
  auto output_data = nlohmann::json::array();
  output_data.push_back(nlohmann::json());
  output_data.at(0)["name"] = grammar_name;
  auto rules = nlohmann::json::array();
  for (unsigned int i = 0; i < productions.size(); i++)
  {
    nlohmann::json individual_rule;
    individual_rule["LHS"] = productions.at(i).lhs.getIdentifier();
    auto rhs_terminals = nlohmann::json::array();
    for (unsigned int j = 0; j < productions.at(i).rhs.size(); j++)
    {
      rhs_terminals.push_back(productions.at(i).rhs.at(j)->getIdentifier());
    }
    individual_rule["RHS"] = rhs_terminals;
    rules.push_back(individual_rule);
  }
  
  output_data.at(0)["rules"] = rules;
  
  nlohmann::json alphabet;

  alphabet["startsymbol"] = "";
  for (unsigned int i = 0; i < nonterminal_alphabet.size(); i++)
  {
    if (nonterminal_alphabet.at(i)->isStartSymbol())
    {
      alphabet["startsymbol"] = nonterminal_alphabet.at(i)->getIdentifier();
    }
  }

  auto nonterminal_json = nlohmann::json::array();
  for (unsigned int i = 0; i < nonterminal_alphabet.size(); i++)
  {
    nonterminal_json.push_back(nonterminal_alphabet.at(i)->getIdentifier());
  }

  alphabet["nonterminals"] =  nonterminal_json;

  auto terminal_json = nlohmann::json::array();
  for (unsigned int i = 0; i < terminal_alphabet.size(); i++)
  {
    terminal_json.push_back(terminal_alphabet.at(i)->getIdentifier());
  }

  alphabet["terminals"] = terminal_json;

  output_data.at(0)["alphabet"] = alphabet;

  std::ifstream input_file;

  auto old_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->grammar_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> old_data;

      input_file.close();
    }
    else
    {
      std::cout << "Not open!\n";
      //old_data
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->grammar_file_name << "\n";
  }

  for (unsigned int i = 0; i < old_data.size(); i++)
  {
    if (!(output_data.at(0)["name"] == old_data.at(i)["name"]))
    {
      output_data.push_back(old_data.at(i));
    }
  }

  std::ofstream output_file(this->grammar_file_name);
  output_file << output_data << std::endl;
  output_file.close();
}

bool GrammarConverter::grammar_exists(std::string grammar_name)
{
  std::ifstream input_file;

  auto old_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->grammar_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> old_data;

      input_file.close();
    }
    else
    {
      std::cout << "Not open!\n";
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->grammar_file_name << "\n";
  }

  for (unsigned int i = 0; i < old_data.size(); i++)
  {
    if (grammar_name == old_data.at(i)["name"]) return true;
  }

  return false;
}

std::vector<wxString> GrammarConverter::get_grammar_names()
{
  std::ifstream input_file;

  auto file_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->grammar_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> file_data;

      input_file.close();
    }
    else
    {
      std::cout << "Not open!\n";
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->grammar_file_name << "\n";
  }

  std::vector<wxString> grammar_names;

  for (unsigned int i = 0; i < file_data.size(); i++)
  {
    std::string temp_name = file_data.at(i)["name"];
    grammar_names.push_back(wxString(temp_name));
  }
  return grammar_names;
}

bool GrammarConverter::delete_grammar(std::string grammar_name)
{
  auto output_data = nlohmann::json::array();

  std::ifstream input_file;

  auto old_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->grammar_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> old_data;

      input_file.close();
    }
    else
    {
      std::cout << "Not open!\n";
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->grammar_file_name << "\n";
    return false;
  }

  for (unsigned int i = 0; i < old_data.size(); i++)
  {
    if (!(grammar_name == old_data.at(i)["name"]))
    {
      output_data.push_back(old_data.at(i));
    }
  }

  std::ofstream output_file(this->grammar_file_name);
  output_file << output_data << std::endl;
  output_file.close();
  return true;
}

std::vector<Terminal*> GrammarConverter::get_terminal_alphabet_from_grammar(
    FormalGrammar grammar)
{
  std::vector<Terminal*> terminals;

  return terminals;
}

bool symbol_in_vector(std::vector<Symbol*> symbols, std::string identifier)
{
  for (unsigned int i = 0; i < symbols.size(); i++)
  {
    if (symbols.at(i)->getIdentifier() == identifier) return true;
  }
  return false;
}

bool GrammarConverter::terminal_in_vector(std::vector<Terminal*> terminals,
                        std::string identifier)
{
  for (unsigned int i = 0; i < terminals.size(); i++)
  {
    if (terminals.at(i)->getIdentifier() == identifier)
    {
      return true;
    }
  }
  return false;
}

bool GrammarConverter::nonterminal_in_vector(
    std::vector<Nonterminal*> nonterminals,
                           std::string identifier)
{
  for (unsigned int i = 0; i < nonterminals.size(); i++)
  {
    if (nonterminals.at(i)->getIdentifier() == identifier)
    {
      return true;
    }
  }
  return false;
}
