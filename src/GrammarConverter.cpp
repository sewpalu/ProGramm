#include "GrammarConverter.hpp"


GrammarConverter::GrammarConverter(wxWindow* msg_dialog_parent)
{
  std::cout << "Constructor\n";
}


GrammarConverter::~GrammarConverter()
{
}

FormalGrammar GrammarConverter::load_grammar_from_std_file(
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
      grammars_data << input_file;

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

  Nonterminal start_symbol;
  std::vector<Production> productions;

  for (unsigned int grammar_number = 0; grammar_number < grammars_data.size();
       grammar_number++)
  {
    if (grammars_data.at(grammar_number) == grammar_name)
    {
      start_symbol =
          Nonterminal(grammars_data.at(grammar_number)["startsymbol"]);

      for (unsigned int j = 0;
           j < grammars_data.at(grammar_number)["rules"].size(); j++)
      {
        std::string lhs_identifier =
            grammars_data.at(grammar_number)["rules"].at(j)["LHS"];
        
        std::vector<std::string> rhs_identifiers =
            grammars_data.at(grammar_number)["rules"].at(j)["RHS"];

        std::vector<Symbol*> rhs_symbols;

        for (unsigned int k = 0; k < rhs_identifiers.size(); k++)
        {
          std::vector<std::string> nonterminals =
              grammars_data.at(grammar_number)["nonterminals"];

          std::vector<std::string> terminals =
              grammars_data.at(grammar_number)["terminals"];

          if (std::find(nonterminals.begin(), nonterminals.end(),
                        rhs_identifiers.at(k)) != nonterminals.end())
          {
            rhs_symbols.push_back(new Nonterminal(rhs_identifiers.at(k)));
          }
          else if (std::find(terminals.begin(), terminals.end(),
                             rhs_identifiers.at(k)) != terminals.end())
          {
            rhs_symbols.push_back(
                new Terminal(rhs_identifiers.at(k), rhs_identifiers.at(k)));
          }
          else
          {
            std::cout << "The grammar contains a symbol in its rules that is "
                         "not contained in its alphabet.\n";
          }
        }

        productions.push_back(Production(Nonterminal(lhs_identifier), rhs_symbols));
      }
    }
  }

  return FormalGrammar(start_symbol, productions);
}

void GrammarConverter::save_grammar_to_std_file(std::vector<Nonterminal*> nonterminal_alphabet,
                                                std::vector<Terminal*> terminal_alphabet,
                                                std::vector<Production> productions,
                                                std::string grammar_name)
{
  std::cout << "Saving to file\n";
  auto output_data = nlohmann::json::array();
  output_data.push_back(nlohmann::json());
  output_data.at(0)["name"] = grammar_name;
  auto rules = nlohmann::json::array();
  for (unsigned int i = 0; i < productions.size(); i++)
  {
    std::cout << i << "\n\n";
    nlohmann::json individual_rule;
    individual_rule["LHS"] = productions.at(i).lhs.getIdentifier();
    auto rhs_terminals = nlohmann::json::array();
    std::cout << typeid(rhs_terminals).name() << " ================= \n";
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

  output_data.at(0)["Alphabet"] = alphabet;

  std::cout << "Opening file\n";
  std::ifstream input_file;

  auto old_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->grammar_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      old_data << input_file;

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
      old_data << input_file;

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
      file_data << input_file;

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
      old_data << input_file;

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