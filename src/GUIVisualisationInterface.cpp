#include "GUIVisualisationInterface.hpp"
#include <bits/c++config.h>

std::size_t GUIVisualisationInterface::Tree::n_leaves() const
{
  int numberOfLeaves = 0;

  for (unsigned int i = 0; i < this->children.size(); i++)
  {
    if (this->children.at(i).children.size() == 0)
    {
      numberOfLeaves++;
    }
    else
    {
      numberOfLeaves += this->children.at(i).n_leaves();
    }
  }

  return numberOfLeaves;
}

std::size_t GUIVisualisationInterface::Tree::n_leaves(std::size_t level) const
{
  int wipLevel = 0;

  int numberOfNodes = 0;

  if (wipLevel == level)
  {
    return 1;
  }
  else if (wipLevel < level)
  {
    for (unsigned int i = 0; i < this->children.size(); i++)
    {
      numberOfNodes +=
          n_leaves_level_helper(wipLevel, level, this->children.at(i));
    }
  }

  return numberOfNodes;
}

std::size_t GUIVisualisationInterface::Tree::depth() const
{
  int depth = 1;

  std::vector<int> depths;

  for (unsigned int i = 0; i < this->children.size(); i++)
  {
    depths.push_back(depth_helper(this->children.at(i), depth));
  }

  for (unsigned int i = 0; i < depths.size(); i++)
  {
    if (depth < depths.at(i))
      depth = depths.at(i);
  }

  return depth;
}

std::size_t GUIVisualisationInterface::Tree::n_leaves_level_helper(
    std::size_t wipLevel, std::size_t level, const Tree& workingNode) const
{
  wipLevel++;

  int numberOfNodes = 0;

  if (wipLevel == level)
  {
    return 1;
  }
  else if (wipLevel < level)
  {
    for (unsigned int i = 0; i < workingNode.children.size(); i++)
    {
      numberOfNodes +=
          n_leaves_level_helper(wipLevel, level, workingNode.children.at(i));
    }
  }

  return numberOfNodes;
}

std::size_t GUIVisualisationInterface::Tree::depth_helper(
    const Tree& working_tree, std::size_t acc_depth) const
{
  acc_depth++;

  if (working_tree.children.size() == 0)
    return acc_depth;

  std::vector<int> depths;

  for (unsigned int i = 0; i < working_tree.children.size(); i++)
  {
    depths.push_back(depth_helper(working_tree.children.at(i), acc_depth));
  }

  int depth = 0;

  for (unsigned int i = 0; i < depths.size(); i++)
  {
    if (depth < depths.at(i))
      depth = depths.at(i);
  }

  return depth;
}

bool GUIVisualisationInterface::Coord::operator<(const Coord& other) const
{
  if (y < other.y)
    return true;
  if (y == other.y && x < other.x)
    return true;
  return false;
}
