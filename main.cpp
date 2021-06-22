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

  std::vector<FormalGrammar> test_grammars;

  for (size_t j = 0; j < 5; j++)
  {
    test_grammars.push_back(test_handler.generate_random_grammar(10,10,10));
  }
  std::cout << "Startsymbol: "
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
  std::cout << "\n";

  int number_of_passes = 20;

  std::ofstream log_file;
  log_file.open("cyk_log.csv");
  log_file << "length,grammar,pass,microseconds\n";

  for (size_t length = 1; length < 21; length++)
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
      }
    }
  }

  log_file.close();

  //Used for starting the GUI
  /*auto cli = ArgumentsDispatcher{argc, argv};
  cli.dispatch();*/
  std::cout << "Tests passed!\n";
}
