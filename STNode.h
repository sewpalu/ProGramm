#pragma once

#include "Symbol.h"
#include <vector>

class STNode
{

public:
	std::vector<STNode> children;
	Symbol value;

	STNode();
	~STNode();
	STNode(Symbol value_input);
	STNode(Symbol value_input, std::vector<STNode> children);

	void addChildren(std::vector<STNode> children_input);

};

