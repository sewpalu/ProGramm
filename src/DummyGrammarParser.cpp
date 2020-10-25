#include "DummyGrammarParser.hpp"
#include <initializer_list>

const auto DummyGrammarParser::demo_grammar = FormalGrammar{
    Nonterminal{"S"},
    {
        {Nonterminal{"S"}, {{new Nonterminal{"A"}, new Nonterminal{"B"}}}},
        {Nonterminal{"A"}, {{new Nonterminal{"B"}, new Nonterminal{"C"}}}},
        {Nonterminal{"B"}, {{new Nonterminal{"B"}, new Nonterminal{"C"}}}},
        {Nonterminal{"C"}, {{new Nonterminal{"C"}, new Nonterminal{"A"}}}},
        {Nonterminal{"A"},
         {std::initializer_list<Symbol*>{new Terminal{"a", "a"}}}},
        {Nonterminal{"B"},
         {std::initializer_list<Symbol*>{new Terminal{"b", "b"}}}},
        {Nonterminal{"C"},
         {std::initializer_list<Symbol*>{new Terminal{"c", "c"}}}},
    }};

const auto DummyGrammarParser::demo_production =
    Production{Nonterminal{{}}, {}};
