#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "CYKLink.hpp"
#include "Visualiser.hpp"

class CYKVisualiser
: public Visualiser
{

public:
	using Matrix = std::vector<std::vector<std::vector<CYKLink>>>;

public:
        // FIXME Why is this public
	Matrix matrix;

public:

	CYKVisualiser(unsigned int wordlength);
	~CYKVisualiser();

	void accept(VisualisationVisitor& visitor) const override
	{
	  visitor.visitCYKVisualiser(*this);
	}

	void setResult(std::pair<unsigned int, unsigned int> position, std::vector<CYKLink> production);

	void dumpContent();

	void dumpContent(unsigned int coordinate1, unsigned int coordinate2, std::string nonterminalIdentifier);

};

