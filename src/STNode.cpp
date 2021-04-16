#include "STNode.h"

STNode::STNode()
{
}

STNode::~STNode()
{
}

STNode::STNode(Symbol value_input)
{
	this->value = value_input;
}

STNode::STNode(Symbol value_input, std::vector<STNode> children_input)
{
	this->value = value_input;
	this->children = children_input;
}

void STNode::addChildren(std::vector<STNode> children_input)
{
	for(int i = 0; i < children_input.size(); i++)
	{
		this->children.push_back(children_input.at(i));
	}
}
