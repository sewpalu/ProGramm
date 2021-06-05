#pragma once

#include <memory>
#include <vector>

#include "STNode.hpp"
#include "STVisualiser.hpp"
#include "Visualisable.hpp"

class SyntaxTree : public Visualisable
{

private:
  std::shared_ptr<const STNode> root;

public:
  explicit SyntaxTree(STNode root_input);
  ~SyntaxTree();
  SyntaxTree();

  std::vector<STNode> getChildren() const;

  STNode getRoot() const;

  int getNumberOfLeaves() const;
};
