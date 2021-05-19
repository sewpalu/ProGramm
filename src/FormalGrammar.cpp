#include "FormalGrammar.hpp"

FormalGrammar::FormalGrammar(Nonterminal start_input,
                             std::vector<Production> rules_input)
    : rules(rules_input), start(start_input)
{
}

FormalGrammar::~FormalGrammar()
{
}
