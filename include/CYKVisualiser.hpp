#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "CYKLink.h"

class CYKVisualiser
{

public:
	std::vector<std::vector<std::vector<CYKLink>>> matrix;

	CYKVisualiser(unsigned int wordlength);
	~CYKVisualiser();

	void setResult(std::pair<unsigned int, unsigned int> position, std::vector<CYKLink> production);

	void dumpContent();

	void dumpContent(unsigned int coordinate1, unsigned int coordinate2, std::string nonterminalIdentifier);

};

