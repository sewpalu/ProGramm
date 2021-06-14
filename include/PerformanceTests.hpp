#pragma once

//Used for test grammars
#include "FormalGrammar.hpp"

//Used for test words
#include "Word.hpp"

class PerformanceTests
{
public:
  PerformanceTests();
  ~PerformanceTests();

  FormalGrammar generate_random_grammar(int number_of_nonterminals,
                                        int number_of_terminals,
                                        int number_of_extra_productions);

  Word generate_random_word(FormalGrammar grammar, int word_length);
  Word generate_included_word(FormalGrammar grammar, int depth);

  Word generate_included_word_with_length(FormalGrammar grammar, int length);

  void run(FormalGrammar gramm, int number_of_passes);

private:
  std::vector<Terminal> generate_part_word(FormalGrammar grammar,
                                           Nonterminal root,
                                           int remaining_depth);

  std::vector<Terminal> generate_part_word_with_length(FormalGrammar grammar,
                                                       Nonterminal root,
                                                       int length);
};