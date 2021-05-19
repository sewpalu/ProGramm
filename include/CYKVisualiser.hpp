#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "CYKLink.hpp"
#include "FormalGrammar.hpp"
#include "STNode.hpp"
#include "SyntaxTree.hpp"

class CYKVisualiser : public Visualiser
{

public:
  std::vector<std::vector<std::vector<CYKLink>>> matrix;

  CYKVisualiser(unsigned int wordlength);
  CYKVisualiser() = default;
  ~CYKVisualiser();

  void setResult(std::pair<unsigned int, unsigned int> position,
                 std::vector<CYKLink> production);

  void dumpContent();

  void dumpAll();

  void dumpContent(unsigned int coordinate1, unsigned int coordinate2,
                   std::string nonterminalIdentifier);

  std::vector<SyntaxTree> convertToSyntaxTree(FormalGrammar grammar);

  // std::pair<std::pair<std::pair<unsigned int, unsigned int>,
  // std::pair<unsigned int, unsigned int>>, std::pair<Symbol, Symbol>>
  // getProductionSource();

  void accept(VisualisationVisitor& visitor) const override
  {
    visitor.visitCYKVisualiser(*this);
  }

  std::unique_ptr<Visualiser> clone() override
  {
    return std::make_unique<CYKVisualiser>(*this);
  }

private:
  std::vector<STNode> addChildrenFromLink(CYKLink inputLink);

};
