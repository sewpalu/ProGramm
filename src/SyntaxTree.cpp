#include "SyntaxTree.hpp"

SyntaxTree::SyntaxTree(STNode root_input)
      : Visualisable(std::unique_ptr<Visualiser>{new STVisualiser{root}})
{
	this->root = root_input;
}

SyntaxTree::~SyntaxTree()
{
}

SyntaxTree::SyntaxTree()
      : Visualisable(std::unique_ptr<Visualiser>{new STVisualiser{root}})
{
}

STNode SyntaxTree::getRoot()
{
	return this->root;
}

std::vector<STNode> SyntaxTree::getChildren()
{
	return this->root.children;
}
