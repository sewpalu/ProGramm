#include "CYKLink.hpp"

Nonterminal CYKLink::getRoot() const
{
    return this->root;
}

std::vector<std::pair<std::pair<unsigned int, unsigned int>, CYKLink>> CYKLink::getProductions() const
{
    return this->productions;
}

CYKLink::CYKLink(Nonterminal root_input, std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>> productions_input)
  : root(root_input), productions(productions_input)
{
}

CYKLink::CYKLink(Nonterminal root_input)
  : root(root_input)
{
}

CYKLink::~CYKLink()
{
}

void CYKLink::setRoot(Nonterminal root_input)
{
    this->root = root_input;
}

void CYKLink::setProductions(std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>> productions_input)
{
    this->productions = productions_input;
}

void CYKLink::addProduction(std::pair<std::pair<unsigned int, unsigned int>, CYKLink> production_input)
{
    this->productions.push_back(production_input);
}

void CYKLink::emptyProductions()
{
    this->productions.clear();
}

bool CYKLink::isEmpty()
{
    bool result = false;
    if (this->root.identifier != "")
    {
        result = true;
    }
    return result;
}

bool CYKLink::equals(CYKLink compare)
{
  bool equals = this->root.identifier == compare.getRoot().identifier;

  if (!equals)
    return false;

  equals = this->getProductions().size() == compare.getProductions().size();

  if (!equals)
    return false;

  for (size_t i = 0; i < this->productions.size(); i++)
  {
    if (!(this->productions.at(i).second.equals(
          compare.getProductions().at(i).second)))
    {
      return false;
    }

    if (!(this->productions.at(i).first.second ==
               compare.getProductions().at(i).first.second))
    {
      return false;
    }

    if (!(this->productions.at(i).second.equals(compare.getProductions().at(i).second)))
    {
      return false;
    }
  }

  return true;
}
