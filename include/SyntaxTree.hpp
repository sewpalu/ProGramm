#pragma once

#include <vector>

#include "STNode.hpp"
#include "STVisualiser.hpp"
#include "Visualisable.hpp"

class SyntaxTree : public Visualisable
{

private:
	STNode root;

public:
	explicit SyntaxTree(STNode root_input);
	~SyntaxTree();
	SyntaxTree();

	STNode getRoot();
	std::vector<STNode> getChildren();
};

