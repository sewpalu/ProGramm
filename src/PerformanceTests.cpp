#include "PerformanceTests.hpp"

#include <random>

PerformanceTests::PerformanceTests()
{
}

PerformanceTests::~PerformanceTests()
{
}

FormalGrammar PerformanceTests::generate_random_grammar(
    int number_of_nonterminals, int number_of_terminals, int number_of_extra_productions)
{

  std::random_device rd;
  std::default_random_engine en(rd());
  std::uniform_int_distribution<int> distr(0, 25);

  //To more easily keep track of all previously used identifiers
  std::vector<std::string> terminals;
  std::vector<std::string> nonterminals;

  std::vector<Production> productions;

  char start_identifier = 'a' + distr(en) % 26;
  nonterminals.push_back(std::string(1, start_identifier));

  //Fill the vector with all Nonterminals of an alphabet
  for (size_t j = 0; j < number_of_terminals; j++)
  { 
    while (true)
    {
      std::string identifier = std::string(1, 'a' + distr(en));
      bool exists = false;
      for (size_t i = 0; i < terminals.size(); i++)
      {
        if (terminals.at(i) == identifier)
        {
          exists = true;
          break;
        }
      }
      for (size_t i = 0; i < nonterminals.size(); i++)
      {
        if (nonterminals.at(i) == identifier)
        {
          exists = true;
          break;
        }
      }
      if (!exists)
      {
        terminals.push_back(identifier);
        break;
      }
    }
  }
  
  //Fill the vector with all Terminals of an alphabet
  for (size_t j = 0; j < number_of_nonterminals - 1; j++)
  {
    while (true)
    {
      std::string identifier = std::string(1, 'a' + distr(en));
      bool exists = false;
      for (size_t i = 0; i < nonterminals.size(); i++)
      {
        if (nonterminals.at(i) == identifier)
        {
          exists = true;
          break;
        }
      }
      for (size_t i = 0; i < terminals.size(); i++)
      {
        if (terminals.at(i) == identifier)
        {
          exists = true;
          break;
        }
      }
      if (!exists)
      {
        nonterminals.push_back(identifier);
        break;
      }
    }
  }

  productions.push_back(Production(
      Nonterminal(std::string(1, start_identifier)),
      {new Nonterminal(nonterminals.at(rand() % nonterminals.size())),
       new Nonterminal(nonterminals.at(rand() % nonterminals.size()))}));
  
  for (size_t i = 0; i < nonterminals.size(); i++)
  {
    productions.push_back(Production(
        Nonterminal(nonterminals.at(i)),
        {new Nonterminal(nonterminals.at(rand() % nonterminals.size())),
         new Nonterminal(nonterminals.at(rand() % nonterminals.size()))}));
  }

  for (size_t j = 0; j < number_of_extra_productions; j++)
  {
    productions.push_back(Production(Nonterminal(nonterminals.at(rand() % nonterminals.size())),
        {new Nonterminal(nonterminals.at(rand() % nonterminals.size())),
         new Nonterminal(nonterminals.at(rand() % nonterminals.size()))}));
  }

  for (size_t j = 0; j < nonterminals.size(); j++)
  {
    std::string terminal_identifier =
        terminals.at(j % terminals.size());
    productions.push_back(
        Production(Nonterminal(nonterminals.at(j)),
                   {new Terminal(terminal_identifier, terminal_identifier)}));
  }

  FormalGrammar return_grammar =
      FormalGrammar{productions, Nonterminal(std::string(1, start_identifier))};

  return return_grammar;
}

void PerformanceTests::run(FormalGrammar gramm, int number_of_passes)
{
}

Word PerformanceTests::generate_random_word(
    FormalGrammar grammar,
                                 int word_length)
{
  //If there were no rules producing terminals, this would return an empty word and show an error message
  bool has_terminal_rule = false;
  for (size_t i = 0; i < grammar.rules.size(); i++)
  {
    if (grammar.rules.at(i).rhs().size() == 1)
    {
      has_terminal_rule = true;
      break;
    }
  }
  if (!has_terminal_rule)
  {
    std::cout << "The grammar entered for generating a random word does not "
                 "have rules producing just terminals.\n";
    std::cout << "Therfore an empty word is returned\n\n";
    return Word();
  }

  std::vector<Terminal> content;
  for (size_t i = 0; i < word_length; i++)
  {
    Terminal addition;

    while (true)
    {
      int rule_number = rand() % grammar.rules.size();

      if (grammar.rules.at(rule_number).rhs().size() == 1)
      {
        addition = Terminal(
            grammar.rules.at(rule_number).rhs().at(0)->getIdentifier(),
            grammar.rules.at(rule_number).rhs().at(0)->getIdentifier());
        break;
      }
    }

    content.push_back(addition);
  }
  return Word(content);
}

Word PerformanceTests::generate_included_word(FormalGrammar grammar, int depth)
{
  std::vector<Terminal> content;

  int rule_number = 0;

  while (true)
  {
    int test_rule = rand() % grammar.rules.size();
    if ((grammar.rules.at(test_rule).rhs().size() == 2) &&
        (grammar.rules.at(test_rule).lhs().getIdentifier() ==
         grammar.start.getIdentifier()))
    {
      rule_number = test_rule;
      break;
    }
  }

  for (size_t i = 0; i < grammar.rules.at(rule_number).rhs().size(); i++)
  {
    std::vector<Terminal> temp = generate_part_word(
        grammar,
        Nonterminal(grammar.rules.at(rule_number).rhs().at(i)->getIdentifier()),
        depth);
    for (size_t i = 0; i < temp.size(); i++)
    {
      content.push_back(temp.at(i));
    }
  }

  return Word(content);
}


std::vector<Terminal> PerformanceTests::generate_part_word(
    FormalGrammar grammar,
                                         Nonterminal root,
                                         int remaining_depth)
{
  std::vector<Terminal> content;

  if (remaining_depth > 1)
  {
    int rule_number = 0;

    while (true)
    {
      int test_rule = rand() % grammar.rules.size();
      if ((grammar.rules.at(test_rule).rhs().size() == 2) &&
          (grammar.rules.at(test_rule).lhs().getIdentifier() ==
           root.getIdentifier()))
      {
        rule_number = test_rule;
        break;
      }
    }

    for (size_t i = 0; i < grammar.rules.at(rule_number).rhs().size(); i++)
    {
      std::vector<Terminal> temp = generate_part_word(grammar, Nonterminal(grammar.rules.at(rule_number).rhs().at(i)->getIdentifier()), remaining_depth - 1);
      for (size_t j = 0; j < temp.size(); j++)
      {
        content.push_back(temp.at(j));
      }
    }
  }
  else
  {
    for (size_t i = 0; i < grammar.rules.size(); i++)
    {
      if ( (grammar.rules.at(i).rhs().size() == 1) &&
          (grammar.rules.at(i).lhs().getIdentifier() == root.getIdentifier()))
      {
        std::string identifier =
            grammar.rules.at(i).rhs().at(0)->getIdentifier();
        content.push_back(Terminal(identifier, identifier));
      }
    }
  }

  return content;
}

Word PerformanceTests::generate_included_word_with_length(FormalGrammar grammar, int length)
{
  std::vector<Terminal> content;


  if (length == 1)
  {
    int rule_number = 0;
    while (true)
    {
      int test_rule = rand() % grammar.rules.size();
      if ((grammar.rules.at(test_rule).rhs().size() == 1) &&
          (grammar.rules.at(test_rule).lhs().getIdentifier() ==
           grammar.start.getIdentifier()))
      {
        rule_number = test_rule;
        break;
      }
    }
    std::string identifier =
        grammar.rules.at(rule_number).rhs().at(0)->getIdentifier();
    content.push_back(Terminal(identifier, identifier));
  }
  else
  {
    int rule_number = 0;

    while (true)
    {
      int test_rule = rand() % grammar.rules.size();
      if ((grammar.rules.at(test_rule).rhs().size() == 2) &&
          (grammar.rules.at(test_rule).lhs().getIdentifier() ==
           grammar.start.getIdentifier()))
      {
        rule_number = test_rule;
        break;
      }
    }
    
    int left_length = 0;
    int right_length = 0;

    if (length % 2 == 0)
    {
      left_length = length / 2;
      right_length = length / 2;
    }
    else
    {
      left_length = length / 2 + 1;
      right_length = length / 2;
    }

    std::vector<Terminal> left_part = generate_part_word_with_length(
        grammar,
                                       Nonterminal(grammar.rules.at(rule_number)
                                                       .rhs()
                                                       .at(0)
                                                       ->getIdentifier()),
                                       left_length);
    for (size_t i = 0; i < left_part.size(); i++)
    {
      content.push_back(left_part.at(i));
    }

    std::vector<Terminal> right_part = generate_part_word_with_length(
        grammar,
                                       Nonterminal(grammar.rules.at(rule_number)
                                                       .rhs()
                                                       .at(1)
                                                       ->getIdentifier()),
                                       right_length);
    for (size_t i = 0; i < right_part.size(); i++)
    {
      content.push_back(right_part.at(i));
    }
  }


  return Word(content);
}

std::vector<Terminal> PerformanceTests::generate_part_word_with_length(FormalGrammar grammar, Nonterminal root,
                                    int length)
{
  std::vector<Terminal> content;

  if (length == 1)
  {
    for (size_t i = 0; i < grammar.rules.size(); i++)
    {
      if (grammar.rules.at(i).rhs().size() == 1)
      {
        if (grammar.rules.at(i).lhs().getIdentifier() == root.getIdentifier())
        {
          content.push_back(
              Terminal(grammar.rules.at(i).rhs().at(0)->getIdentifier(),
                       grammar.rules.at(i).rhs().at(0)->getIdentifier()));
          return content;
        }
      }
    }
  }
  else
  {
    int left_length = 0;
    int right_length = 0;

    if (length % 2 == 0)
    {
      left_length = length / 2;
      right_length = length / 2;
    }
    else
    {
      left_length = length / 2 + 1;
      right_length = length / 2;
    }

    int rule_number = 0;

    while (true)
    {
      int test_rule = rand() % grammar.rules.size();
      if ((grammar.rules.at(test_rule).rhs().size() == 2) &&
          (grammar.rules.at(test_rule).lhs().getIdentifier() ==
           root.getIdentifier()))
      {
        rule_number = test_rule;
        break;
      }
    }

    std::vector<Terminal> left_part = generate_part_word_with_length(
        grammar,
        Nonterminal(grammar.rules.at(rule_number).rhs().at(0)->getIdentifier()),
        left_length);

    std::vector<Terminal> right_part = generate_part_word_with_length(
        grammar,
        Nonterminal(grammar.rules.at(rule_number).rhs().at(1)->getIdentifier()),
        right_length);

    /*std::cout << "Left check: " << left_length << " vs. "
              << left_part.size() << "\n";
    std::cout << "Right check: " << right_length << " vs. " << right_part.size()
              << "\n";*/

    for (size_t j = 0; j < left_part.size(); j++)
    {
      content.push_back(left_part.at(j));
    }
    for (size_t j = 0; j < right_part.size(); j++)
    {
      content.push_back(right_part.at(j));
    }
  }

  return content;
}