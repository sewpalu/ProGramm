#pragma once

#include "Symbol.hpp"
#include <memory>
#include <vector>
#include <iostream>

class STNode
{

public:
  std::vector<STNode> children;
  std::unique_ptr<Symbol> value;

  STNode();
  ~STNode();
  explicit STNode(std::unique_ptr<Symbol> value_input);
  STNode(std::unique_ptr<Symbol> value_input, std::vector<STNode> children);

  STNode(const STNode& other)
  {
    *this = other;
  }

  STNode& operator=(const STNode& other)
  {
    children = other.children;
    value = other.value->clone();
    return *this;
  }

  void addChildren(std::vector<STNode> children_input);

  int getNumberOfLeaves() const;
  int getMaxDepth() const;
  int getNumberOfNodesOnLevel(int level) const;

  std::string getText() const;

private:
  int recursiveGetMaxDepth(STNode workingNode, int previousDepth) const;
  int recursiveNodesOnLevel(int wipLevel, int level, STNode workingNode) const;
};
