#include "SyntaxTree.h"

SyntaxTree::SyntaxTree(STNode root_input)
{
	this->root = root_input;
}

SyntaxTree::~SyntaxTree()
{
}

SyntaxTree::SyntaxTree()
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

void SyntaxTree::dumpTree()
{
	std::cout << "Dump Tree: \n";
	std::cout << this->root.value.getIdentifier() << "\n";
	for (unsigned int i = 0; i < this->getChildren().size(); i++)
	{
		std::cout << this->getChildren().at(i).value.getIdentifier() << "      ";
		for (unsigned int j = 0; j < this->getChildren().at(i).children.size(); j++)
		{
			if (j > 0) std::cout << "      ";
			std::cout << this->getChildren().at(i).children.at(j).value.getIdentifier() << "      ";
			for (unsigned int k = 0; k < this->getChildren().at(i).children.at(j).children.size(); k++)
			{
				if (k > 0) std::cout << "             ";
				std::cout << this->getChildren().at(i).children.at(j).children.at(k).value.getIdentifier() << "            ";
				try
				{
					for (unsigned int l = 0; l < this->getChildren().at(i).children.at(j).children.at(k).children.size(); l++)
					{
						if (l > 0) std::cout << "                   ";
						std::cout << this->getChildren().at(i).children.at(j).children.at(k).children.at(k).value.getIdentifier() << "      ";

						for (unsigned int m = 0; m < this->getChildren().at(i).children.at(j).children.at(k).children.at(l).children.size(); m++)
						{
							std::cout << this->getChildren().at(i).children.at(j).children.at(k).children.at(l).children.at(m).value.getIdentifier() << "\n";
						}
						std::cout << "\n";
					}
				}
				catch (const std::exception&)
				{
					std::cout << "\n";
				}
				if (this->getChildren().at(i).children.at(j).children.at(k).children.size() == 0) std::cout << "\n";
			}
		}
	}
	/*for (unsigned int childCounter = 0; childCounter < this->root.children.size(); childCounter++)
	{

		dumpChild(this->root.children.at(childCounter));
		std::cout << "\n";
	}*/
}

void SyntaxTree::dumpChild(STNode child)
{
	std::cout << child.value.getIdentifier();
	for (unsigned int childCounter = 0; childCounter < child.children.size() ; childCounter++)
	{
		dumpChild(child.children.at(childCounter));
	}
}
