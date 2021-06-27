#include <iostream>
#include <fstream>
#include <chrono>

// Used for starting the GUI
//#include "ArgumentsDispatcher.hpp"

#include "FormalGrammar.hpp"

#include "PerformanceTests.hpp"

#include "CYKAlgorithm.hpp"

int main(int argc, char** argv)
{
  std::vector<Word> test_words;
  PerformanceTests test_handler = PerformanceTests();

  CYKAlgorithm cyk_alg = CYKAlgorithm();

  std::chrono::steady_clock::time_point begin;
  std::chrono::steady_clock::time_point end;

  Nonterminal* q = new Nonterminal("q", true);
  Nonterminal* v = new Nonterminal("v");
  Nonterminal* f = new Nonterminal("f");

  Terminal* j = new Terminal("j", "j");
  Terminal* p = new Terminal("p", "p");
  Terminal* e = new Terminal("e", "e");

  std::cout << "Make productions\n";
  std::vector<Production> prods = {Production(Nonterminal("q", true), {new Nonterminal("f"),new Nonterminal("f")}),
                                  Production(Nonterminal("q", true), {new Nonterminal("v"),new Nonterminal("v")}),
                                  Production(Nonterminal("v"), {new Nonterminal("f"),new Nonterminal("v")}),
                                  Production(Nonterminal("f"), {new Nonterminal("q", true),new Nonterminal("q", true)}),
                                  Production(Nonterminal("f"), {new Nonterminal("v"),new Nonterminal("f")}),
                                  Production(Nonterminal("q", true), {new Nonterminal("f"),new Nonterminal("v")}),
                                  Production(Nonterminal("v"), {new Nonterminal("v"),new Nonterminal("f")}),
                                  Production(Nonterminal("q", true), {new Terminal("j", "j")}),
                                  Production(Nonterminal("v"), {new Terminal("p", "p")}),
                                  Production(Nonterminal("f"), {new Terminal("e", "e")})};

  std::cout << "Make grammar\n";
  FormalGrammar test{prods, *q};
  
  std::cout << "Test word: ";
  //Word test_word = test_handler.generate_included_word_with_length(test, 5);
  Word test_word = Word({*p, *e, *p, *e, *p});
  //Word test_word({*p, *p, *j, *p, *e, *p, *e, *p, *j, *j});

  for (size_t i = 0; i < test_word.getSize(); i++)
  {
    std::cout << " '" << test_word.at(i).getIdentifier() << "'";
  }
  std::cout << "\n";

  std::cout << "Run CYK\n";
  std::vector<SyntaxTree> result = cyk_alg.parse(test, test_word);

  /*std::cout << "Startsymbol: "
            << test_grammars.at(0).start.getIdentifier() << "\n";

  for (size_t i = 0; i < test_grammars.at(0).rules.size(); i++)
  {
    std::cout
        << test_grammars.at(0).rules.at(i).lhs().getIdentifier();
    std::cout << " -> ";
    for (size_t j = 0;
         j < test_grammars.at(0).rules.at(i).rhs().size(); j++)
    {
      std::cout << test_grammars.at(0)
                       .rules.at(i)
                       .rhs()
                       .at(j)
                       ->getIdentifier();
    }
    std::cout << "\n";
  }
  std::cout << "\n";*/

  

  std::vector<FormalGrammar> test_grammars;

  for (size_t j = 0; j < 1; j++)
  {
    test_grammars.push_back(test_handler.generate_random_grammar(5,5,2));
  }

  int number_of_passes = 1;

  std::ofstream log_file;
  log_file.open("cyk_log.csv");
  log_file << "length,grammar,pass,microseconds\n";

  for (size_t length = 1; length < 1; length++)
  {
    std::cout << "Length: " << length << "\n";
    for (size_t grammar_number = 0; grammar_number < test_grammars.size(); grammar_number++)
    {
      std::cout << "      Grammar: " << grammar_number << "\n";

      std::cout << "Startsymbol: "
                << test_grammars.at(grammar_number).start.getIdentifier()
                << "\n";

      for (size_t i = 0; i < test_grammars.at(grammar_number).rules.size(); i++)
      {
        std::cout << test_grammars.at(grammar_number)
                         .rules.at(i)
                         .lhs()
                         .getIdentifier();
        std::cout << " -> ";
        for (size_t j = 0; j < test_grammars.at(grammar_number).rules.at(i).rhs().size(); j++)
        {
          std::cout << test_grammars.at(grammar_number)
                           .rules.at(i)
                           .rhs()
                           .at(j)
                           ->getIdentifier();
        }
        std::cout << "\n";
      }
      std::cout << "\n";


      for (size_t pass = 0; pass < number_of_passes; pass++)
      {
        std::cout << "            Pass: " << pass << "\n";
        //std::cout << "Starting cyk\n";
        Word test_word = test_handler.generate_included_word_with_length(
            test_grammars.at(grammar_number), length);
        std::cout << "Word: ";
        for (size_t i = 0; i < test_word.content.size(); i++)
        {
          std::cout << "'" << test_word.content.at(i).getIdentifier() << "' ";
        }
        std::cout << "\n";

        begin = std::chrono::steady_clock::now();
        std::vector<SyntaxTree> trees = cyk_alg.parse(
            test_grammars.at(grammar_number), test_word);
        //std::cout << "Done with cyk\n";
        end = std::chrono::steady_clock::now();

        log_file << length << "," << grammar_number << "," << pass << ","
                 << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                          begin)
                        .count() << "\n";
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         end - begin)
                         .count()
                  << "microseconds\n";
        std::cout << "Anzahl der Syntaxbäume: " << trees.size() << "\n";
      }
    }
  }

  log_file.close();

  //Used for starting the GUI
  /*auto cli = ArgumentsDispatcher{argc, argv};
  cli.dispatch();*/
  std::cout << "Tests passed!\n";
}
