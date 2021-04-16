#include "CYKLink.h"

Nonterminal CYKLink::getRoot()
{
    return this->root;
}

std::vector<std::pair<std::pair<unsigned int, unsigned int>, CYKLink>> CYKLink::getProductions()
{
    return this->productions;
}

CYKLink::CYKLink(Nonterminal root_input, std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>> productions_input)
{
    this->root = root_input;
    this->productions = productions_input;
}

CYKLink::CYKLink()
{
}

CYKLink::CYKLink(Nonterminal root_input)
{
    this->root = root_input;
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
