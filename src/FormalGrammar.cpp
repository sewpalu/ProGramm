#include "FormalGrammar.hpp"

FormalGrammar::FormalGrammar(Nonterminal start_input, std::vector<Production> rules_input)
{
	this->start = start_input;
	this->rules = rules_input;
}


FormalGrammar::FormalGrammar(std::vector<Nonterminal*> nonterminals,
                             std::vector<Production> rules_input)
{
  for (unsigned int i = 0; i < nonterminals.size(); i++)
  {
    if (nonterminals.at(i)->isStartSymbol())
    {
      this->start = Nonterminal(nonterminals.at(i)->getIdentifier());
      break;
    }
  }
  this->rules = rules_input;
}

FormalGrammar::~FormalGrammar()
{
}
