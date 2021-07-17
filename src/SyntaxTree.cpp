#include "SyntaxTree.hpp"
#include <memory>

SyntaxTree::SyntaxTree(STNode root_input)
    : Visualisable(
          std::make_unique<STVisualiser>(std::make_shared<STNode>(root_input))),
      root(dynamic_cast<const STVisualiser&>(visualiser()).root_node)
{
}

int SyntaxTree::getNumberOfLeaves() const
{
  return this->root->getNumberOfLeaves();
}

const STNode& SyntaxTree::getRoot() const
{
  return *this->root;
}
