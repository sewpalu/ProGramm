#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <typeinfo>

#include "CYKLink.h"
#include "SyntaxTree.h"
#include "STNode.h"
#include "FormalGrammar.h"

class CYKVisualiser
{

public:
	std::vector<std::vector<std::vector<CYKLink>>> matrix;

	CYKVisualiser(unsigned int wordlength);
	CYKVisualiser();
	~CYKVisualiser();

	void setResult(std::pair<unsigned int, unsigned int> position, std::vector<CYKLink> production);

	void dumpContent();

	void dumpAll();

	void dumpContent(unsigned int coordinate1, unsigned int coordinate2, std::string nonterminalIdentifier);

	std::vector<SyntaxTree> convertToSyntaxTree(FormalGrammar grammar);

	//std::pair<std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int>>, std::pair<Symbol, Symbol>> getProductionSource();

private:
	std::vector<STNode> addChildrenFromLink(CYKLink inputLink);

};

