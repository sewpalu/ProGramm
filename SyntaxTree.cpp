#include "SyntaxTree.h"

SyntaxTree::SyntaxTree(STNode root_input)
{
	this->root = root_input;
}

SyntaxTree::~SyntaxTree()
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
