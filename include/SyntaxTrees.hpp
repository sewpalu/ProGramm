#ifndef SYNTAX_TREES_HPP
#define SYNTAX_TREES_HPP

#include <memory>
#include <vector>

#include "STreesVisualiser.hpp"
#include "SyntaxTree.hpp"

/**
 * Wrapper class for multiple SyntaxTrees.
 *
 * Used to make a list of SyntaxTrees visualisable within our program structure.
 */
class SyntaxTrees : public Visualisable
{
public:
  using Trees = std::vector<SyntaxTree>;

private:
  const std::shared_ptr<const Trees> m_trees;

public:
  SyntaxTrees(Trees trees)
      : Visualisable(
            std::make_unique<STreesVisualiser>(std::make_shared<Trees>(trees))),
        m_trees(dynamic_cast<const STreesVisualiser&>(visualiser()).trees)
  {
  }

  const Trees& trees()
  {
    return *m_trees;
  }
};

#endif /* ifndef SYNTAX_TREES_HPP */
