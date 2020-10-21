#pragma once

#include "STNode.h"
#include <vector>

class SyntaxTree
{

private:
	STNode root;

public:
	SyntaxTree(STNode root_input);
	~SyntaxTree();

	STNode getRoot();
	std::vector<STNode> getChildren();
};

