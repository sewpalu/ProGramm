#include "STNode.hpp"

STNode::STNode()
{
}

STNode::~STNode()
{
}

STNode::STNode(std::unique_ptr<Symbol> value_input)
{
	this->value = std::move(value_input);
}

STNode::STNode(std::unique_ptr<Symbol> value_input, std::vector<STNode> children_input)
{
	this->value = std::move(value_input);
	this->children = children_input;
}

void STNode::addChildren(std::vector<STNode> children_input)
{
	for(unsigned int i = 0; i < children_input.size(); i++)
	{
		this->children.push_back(children_input.at(i));
	}
}
