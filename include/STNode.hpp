#pragma once

#include "Symbol.hpp"
#include <iostream>
#include <memory>
#include <vector>

/**
 * Node of a syntax tree.
 *
 * This is a struct on the grounds of C++ Core Guideline C.2
 * See: http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-struct
 */
struct STNode
{
  std::vector<STNode> children;
  std::unique_ptr<Symbol> value;

  /**
   * Constructs a leaf node
   */
  explicit STNode(std::unique_ptr<Symbol> value_input);

  /**
   * constructs a branch node.
   */
  STNode(std::unique_ptr<Symbol> value_input, std::vector<STNode> children);

  /**
   * Copying is non-trivial because of the dynamic subtypes of Symbols.
   */
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

  /**
   * Calculates the number of leaves under this node.
   */
  int getNumberOfLeaves() const;

  /**
   * Calculates the maximum sub-tree depth relative to this node.
   */
  int getMaxDepth() const;

  /**
   * Calculates the number of nodes below this node, at a particular tree depth.
   *
   * \param level Depth relative to this node.
   */
  int getNumberOfNodesOnLevel(int level) const;

  void stupid_dump();

  std::string getText() const;

private:
  int recursiveGetMaxDepth(STNode workingNode, int previousDepth) const;
  int recursiveNodesOnLevel(int wipLevel, int level, STNode workingNode) const;
  void dump_level(int level);
  void rec_dump_level(STNode working_node, int goal_level, int current_level);
};
