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

  std::string file_name = "grammars.json";

  std::cout << "Opening file\n";
  std::ifstream input_file;

  auto old_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(file_name, std::ifstream::in);

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
    std::cout << "Cannot read\n";
  }
  
  std::cout << "Old data size: " << old_data.size() << "\n";

  for (unsigned int i = 0; i < old_data.size(); i++)
  {
    if (!(output_data.at(0)["name"] == old_data.at(i)["name"]))
    {
      output_data.push_back(old_data.at(i));
    }
  }


  std::ofstream output_file(file_name);
  output_file << output_data << std::endl;
  output_file.close();
}
