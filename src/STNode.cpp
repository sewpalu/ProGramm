#include "STNode.hpp"

STNode::STNode(std::unique_ptr<Symbol> value_input)
{
  this->value = std::move(value_input);
}

STNode::STNode(std::unique_ptr<Symbol> value_input,
               std::vector<STNode> children_input)
{
  this->value = std::move(value_input);
  this->children = children_input;
}

void STNode::addChildren(std::vector<STNode> children_input)
{
  for (unsigned int i = 0; i < children_input.size(); i++)
  {
    this->children.push_back(children_input.at(i));
  }
}

int STNode::getNumberOfLeaves() const
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
      numberOfLeaves += this->children.at(i).getNumberOfLeaves();
    }
  }

  return numberOfLeaves;
}

int STNode::getNumberOfNodesOnLevel(int level) const
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
          recursiveNodesOnLevel(wipLevel, level, this->children.at(i));
    }
  }

  return numberOfNodes;
}

int STNode::recursiveNodesOnLevel(int wipLevel, int level,
                                  STNode workingNode) const
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
          recursiveNodesOnLevel(wipLevel, level, workingNode.children.at(i));
    }
  }

  return numberOfNodes;
}

int STNode::getMaxDepth() const
{
  int maxDepth = 1;

  std::vector<int> depths;

  for (unsigned int i = 0; i < this->children.size(); i++)
  {
    depths.push_back(recursiveGetMaxDepth(this->children.at(i), maxDepth));
  }

  for (unsigned int i = 0; i < depths.size(); i++)
  {
    if (maxDepth < depths.at(i))
      maxDepth = depths.at(i);
  }

  return maxDepth;
}

int STNode::recursiveGetMaxDepth(STNode workingNode, int previousDepth) const
{
  previousDepth++;

  if (workingNode.children.size() == 0)
    return previousDepth;

  std::vector<int> depths;

  for (unsigned int i = 0; i < workingNode.children.size(); i++)
  {
    depths.push_back(
        recursiveGetMaxDepth(workingNode.children.at(i), previousDepth));
  }

  int maxDepth = 0;

  for (unsigned int i = 0; i < depths.size(); i++)
  {
    if (maxDepth < depths.at(i))
      maxDepth = depths.at(i);
  }

  return maxDepth;
}

std::string STNode::getText() const
{
  return this->value->identifier;
}

void STNode::stupid_dump()
{
  std::cout << "SyntaxTree stupid dump: \n";
  std::cout << "Level 0: " << this->value->identifier << "\n";
  for (size_t i = 1; i < this->getMaxDepth(); i++)
  {
    std::cout << "Level " << i << ": ";
    for (size_t j = 0; j < this->children.size(); j++)
    {
      rec_dump_level(this->children.at(j), i, 1);
    }
    std::cout << "\n";
  }
  std::cout << "Syntax Tree dump end\n";
}

void STNode::dump_level(int level)
{
  for (size_t i = 0; i < this->children.size(); i++)
  {
    rec_dump_level(this->children.at(i), level, 1);
  }
}

void STNode::rec_dump_level(STNode working_node, int goal_level, int current_level)
{
  if (goal_level == current_level)
  {
    std::cout << working_node.getText() << " | ";
  }
  else
  {
    for (size_t i = 0; i < working_node.children.size(); i++)
    {
      rec_dump_level(working_node.children.at(i), goal_level,
                     current_level + 1);
    }
  }
}
