#include "dummy_grammar_parser.hpp"

const auto DummyGrammarParser::demo_grammar =
    FormalGrammar{{}, Nonterminal{{}}};

const auto DummyGrammarParser::demo_production =
    Production{Nonterminal{{}}, {}};