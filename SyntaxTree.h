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
	SyntaxTree();

	STNode getRoot();
	std::vector<STNode> getChildren();

	void dumpTree();

	void dumpChild(STNode child);
};

