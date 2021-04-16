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
	for(unsigned int i = 0; i < children_input.size(); i++)
	{
		this->children.push_back(children_input.at(i));
	}
}

bool STNode::onlyOneChild()
{
	if (this->children.size() == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool STNode::noChildren()
{
	if (this->children.size() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool STNode::allTerminalLeaves()
{
	bool allLeaves;

	std::cout << "Node check: " << this->value.getIdentifier() << "\n";

	if (this->children.size() == 0)
	{
		std::cout << "Check this: " << this->value.getIdentifier() << " : " << typeid(this->value).name() << " equals? " << typeid(Terminal).name() << "\n";
		if (typeid(this->value) == typeid(Terminal))
		{
			allLeaves = true;
		}
		else
		{
			std::cout << "No children: " << this->value.getIdentifier() << "\n";
			allLeaves = false;
		}
	}
	else
	{
		allLeaves = true;

		for (unsigned int childCounter = 0; childCounter < this->children.size(); childCounter++)
		{
			if (!this->children.at(childCounter).allTerminalLeaves())
			{
				allLeaves = false;
				std::cout << "Leaf report: " << this->value.getIdentifier() << "\n";
			}
		}
	}

	std::cout << "Leaf check over: " << this->value.getIdentifier() << "\n";

	return allLeaves;
}

void STNode::clearChildren()
{
	std::vector<STNode> emptyChildren;
	this->children = emptyChildren;
}
