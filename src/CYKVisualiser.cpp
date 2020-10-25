#include "CYKVisualiser.hpp"

CYKVisualiser::CYKVisualiser(unsigned int wordlength)
{
	std::vector<std::vector<std::vector<CYKLink>>> outerVector;
	std::vector<std::vector<CYKLink>> innerVector1;
	std::vector<CYKLink> innerVector2;
	CYKLink content;
	for (unsigned int i = 0; i < wordlength; i++)
	{
		innerVector2.push_back(content);
	}
	for (unsigned int i = 0; i < wordlength; i++)
	{
		innerVector1.push_back(innerVector2);
	}
	for (unsigned int i = 0; i < wordlength; i++)
	{
		outerVector.push_back(innerVector1);
	}
	this->matrix = outerVector;
}

CYKVisualiser::~CYKVisualiser()
{
}

void CYKVisualiser::setResult(std::pair<unsigned int, unsigned int> position, std::vector<CYKLink> productions) //
{
	for (unsigned int i = 0; i < productions.size(); i++)
	{
		this->matrix.at(position.first).at(position.second).push_back(productions.at(i));
	}
}

void CYKVisualiser::dumpContent()
{
	//std::cout << "Visualiser Content: \n";

	for (int i = this->matrix.size() - 1; i >= 0; i--)
	{
		for (unsigned int j = 0; j < this->matrix.at(i).size() - i; j++)
		{
			std::string displayPlace = "";
			for (unsigned int k = 0; k < this->matrix.at(i).at(j).size(); k++)
			{
				try {
					if (!(displayPlace.find(this->matrix.at(i).at(j).at(k).getRoot().getIdentifier()) != std::string::npos)) {
						if (displayPlace.size() > 0)
						{
							displayPlace = displayPlace + ", ";
						}
						displayPlace = displayPlace + this->matrix.at(i).at(j).at(k).getRoot().getIdentifier();
					}
				}
				catch (...) {
					std::cout << "Error when writing!\n";
					std::cout << "Matrix size i: " << this->matrix.size() << "\n";
					std::cout << "Matrix size j: " << this->matrix.at(i).size() << "\n";
					std::cout << "Matrix size k: " << this->matrix.at(i).at(j).size() << "\n";
					std::cout << "i: " << i << "j: " << j << "k: " << k << "\n";
				}
			}
			std::cout << displayPlace;
			std::cout << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "-----------\n";
}

void CYKVisualiser::dumpContent(unsigned int coordinate1, unsigned int coordinate2, std::string nonterminalIdentifier)
{
	//std::cout << "Visualiser Content: \n";
	std::cout << "Highlighting: (" << coordinate1 << "/" << coordinate2 << ") for Nonterminal " << nonterminalIdentifier << "\n";

	std::pair<unsigned int, unsigned int> dependency1Coordinates;
	std::pair<unsigned int, unsigned int> dependency2Coordinates;


	for (unsigned int i = 0; i < this->matrix.at(coordinate1).at(coordinate2).size(); i++)
	{

		if (this->matrix.at(coordinate1).at(coordinate2).at(i).getRoot().getIdentifier() == nonterminalIdentifier)
		{
			dependency1Coordinates.first = this->matrix.at(coordinate1).at(coordinate2).at(i).getProductions().at(0).first;
			dependency1Coordinates.second = this->matrix.at(coordinate1).at(coordinate2).at(i).getProductions().at(0).second;
			dependency2Coordinates.first = this->matrix.at(coordinate1).at(coordinate2).at(i).getProductions().at(1).first;
			dependency2Coordinates.second = this->matrix.at(coordinate1).at(coordinate2).at(i).getProductions().at(1).second;

			break;
		}
	}

	std::cout << "Visualiser Content: \n";

	for (unsigned int i = this->matrix.size() - 1; i >= 0; i--)
	{
		for (unsigned int j = 0; j < this->matrix.at(i).size() - i; j++)
		{
			if (dependency1Coordinates.first == i)
			{
				if (dependency1Coordinates.second == j)
				{
					std::cout << "P1: ";
				}
			}

			if (dependency2Coordinates.first == i)
			{
				if (dependency2Coordinates.second == j)
				{
					std::cout << "P2: ";
				}
			}

			std::string displayPlace = "";
			for (unsigned int k = 0; k < this->matrix.at(i).at(j).size(); k++)
			{
				try {
					if (!(displayPlace.find(this->matrix.at(i).at(j).at(k).getRoot().getIdentifier()) != std::string::npos)) {
						if (displayPlace.size() > 0)
						{
							displayPlace = displayPlace + ", ";
						}
						displayPlace = displayPlace + this->matrix.at(i).at(j).at(k).getRoot().getIdentifier();
					}
				}
				catch (...) {
					std::cout << "Error when writing!\n";
					std::cout << "Matrix size i: " << this->matrix.size() << "\n";
					std::cout << "Matrix size j: " << this->matrix.at(i).size() << "\n";
					std::cout << "Matrix size k: " << this->matrix.at(i).at(j).size() << "\n";
					std::cout << "i: " << i << "j: " << j << "k: " << k << "\n";
				}
			}
			std::cout << displayPlace;
			std::cout << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "-----------\n";
}

/*void CYKVisualiser::dumpLength()
{

	std::cout << "Visualiser Lengths: \n";

	unsigned int counter;

	for (int i = this->matrix.size() - 1; i >= 0; i--)
	{
		for (unsigned int j = 0; j < this->matrix.at(i).size(); j++)
		{
			counter = 0;
			for (unsigned int k = 0; k <  this->matrix.at(i).at(j).size(); k++)
			{
				if (this->matrix.at(i).at(j).at(k).getProductions().size() > 0)
				{
					counter++;
				}
			}
			std::cout << counter << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "-----------\n";
}*/
