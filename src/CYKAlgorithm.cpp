#include "CYKAlgorithm.hpp"
#include <memory>

std::unique_ptr<Visualiser> CYKAlgorithm::parse(FormalGrammar grammar, Word input) //FormalGrammar grammar, Word input
{
	std::pair<unsigned int, unsigned int> position1 = { 0,0 };
	std::pair<unsigned int, unsigned int> position2 = { 1,1 };

	//std::pair<std::pair<unsigned int, unsigned int>> testPairs = { position1, position2 };

	std::vector<std::pair<unsigned, unsigned int>> testPairs = { position1, position2 };

	//void setResult(std::pair<unsigned int, unsigned int> position, CYKLink production);

	Nonterminal nonS("nonS");

	CYKLink linkTest(nonS, testPairs);

	std::vector<CYKLink> resultTest = { linkTest };

	CYKVisualiser solution(input.getSize());

	Terminal testTerminal;

	std::vector<std::pair<unsigned int, unsigned int>> productionPositions;

	//Iterate through Terminal that make up the input word
	for (unsigned int wordPos = 0; wordPos < input.getSize(); wordPos++)
	{
		testTerminal = input.at(wordPos);

		//Iterate through rules to see if a rule (with a Nonterminal on the left sight) can produce the Terminal
		for (unsigned int rulePos = 0; rulePos < grammar.rules.size(); rulePos++)
		{
			//for first round of CYK only rules with length are relevant, as it's looking for rules that produce
			//exactly one Terminal ("letter" of word)
			if (grammar.rules.at(rulePos).rhs.size() == 1)
			{
				//See Symbol (it's known this is a Terminal) has same Terminal in word
				if (grammar.rules.at(rulePos).rhs.at(0)->identifier == testTerminal.identifier)
				{
					productionPositions.push_back( { rulePos, wordPos });
					CYKLink terminalLink(grammar.rules.at(rulePos).lhs, productionPositions);

					std::vector<CYKLink> productionVector = { terminalLink };
					//First at set to 0, as this is always the case in first line of CYK
					solution.matrix.at(0).at(wordPos) = productionVector;
				}
			}
		}
	}


	solution.dumpContent();

	std::vector<Nonterminal> searchSymbolLeft;
	std::vector<Nonterminal> searchSymbolRight;

	std::vector<CYKLink> tempProductions;
	CYKLink tempResult;
	std::vector<std::pair<unsigned, unsigned int>> tempPositions;

	//std::cout << "Identifier: " << solution.matrix.at(1).at(0).at(0).getRoot().getIdentifier() << "\n";
	
	//Loop to go through CYK Lines from bottom to top
	//Go through line of CYK matrix, start in second (1 in vector) row
	for(unsigned int cykLine = 1; cykLine < input.getSize(); cykLine++)
	{
		//Go through columns of CYK matrix
		//With each line up, there's one column less
		for (unsigned int cykCol = 0; cykCol < input.getSize() - cykLine; cykCol++)
		{
			std::cout << "Checking: (" << cykLine << "/" << cykCol << ") ------------------------------------------------------------------------------------------------------------------\n";
			//Iterate through all possible combinations for Nonterminals on the rightside of the possible production rule
			for (unsigned int combination = 0; combination < cykLine; combination++)
			{
				tempProductions.clear();

				searchSymbolLeft.clear();
				searchSymbolRight.clear();
				
				//int test1 = solution.matrix.at(combination).at(cykCol).size();
				//int test2 = solution.matrix.at(cykLine - combination - 1).at(cykCol + combination).size();

				for (unsigned int nonterminal = 0; nonterminal < solution.matrix.at(combination).at(cykCol).size(); nonterminal++)
				{
					if (solution.matrix.at(combination).at(cykCol).size() > nonterminal)
					{
						searchSymbolLeft.push_back(solution.matrix.at(combination).at(cykCol).at(nonterminal).getRoot());
					}
				}

				for (unsigned int nonterminal = 0; nonterminal < solution.matrix.at(cykLine - combination - 1).at(cykCol + combination + 1).size(); nonterminal++)
				{
					if (solution.matrix.at(cykLine - combination - 1).at(cykCol + combination + 1).size() > nonterminal)
					{
						searchSymbolRight.push_back(solution.matrix.at(cykLine - combination - 1).at(cykCol + combination + 1).at(nonterminal).getRoot());
					}
				}
				unsigned int numberOfLeftNonterminals = searchSymbolLeft.size();
				unsigned int numberOfRightNonterminals = searchSymbolRight.size();

				tempProductions.clear();

				for(unsigned int prodCounter = 0; prodCounter < grammar.rules.size(); prodCounter++)
				{
					if (grammar.rules.at(prodCounter).rhs.size() == 2)
					{
						//std::cout << "Rule: " << grammar.rules.at(prodCounter).lhs.getIdentifier() << "->" << grammar.rules.at(prodCounter).rhs.at(0).getIdentifier() << grammar.rules.at(prodCounter).rhs.at(1).getIdentifier() << "\n";
						
						for (unsigned int numberOfLeftNonterminal = 0; numberOfLeftNonterminal < numberOfLeftNonterminals; numberOfLeftNonterminal++)
						{
							if (grammar.rules.at(prodCounter).rhs.at(0)->getIdentifier() == searchSymbolLeft.at(numberOfLeftNonterminal).getIdentifier())
							{
								for(unsigned int numberOfRightNonterminal = 0; numberOfRightNonterminal < numberOfRightNonterminals; numberOfRightNonterminal++)
								{
									if (grammar.rules.at(prodCounter).rhs.at(1)->getIdentifier() == searchSymbolRight.at(numberOfRightNonterminal).getIdentifier())
									{
										tempResult.setRoot(grammar.rules.at(prodCounter).lhs);
										tempPositions = { { combination, cykCol }, { cykLine - combination - 1, cykCol + combination + 1 } };
										//tempPositions = { { 0, combination }, { combination, input.getSize() - combination } };
										tempResult.setProductions(tempPositions);
										tempProductions.push_back(tempResult);
									}
								}
							}
						}

					}
				}
				if (tempProductions.size() > 0)
				{
					solution.setResult({ cykLine, cykCol }, tempProductions);
				}
				solution.dumpContent();
			}
		}
	}

	bool included = false;

	for (unsigned int nonterminalCounter = 0; nonterminalCounter < solution.matrix.at(input.getSize() - 1).at(0).size(); nonterminalCounter++)
	{
		if (solution.matrix.at(input.getSize() - 1).at(0).at(nonterminalCounter).getRoot().getIdentifier() == "S")
		{
			included = true;
		}
	}

	if (included)
	{
		std::cout << "Wort enthalten!\n";
	}
	else
	{
		std::cout << "Better luck next time!\n";
	}

	return std::make_unique<CYKVisualiser>(solution);
}

CYKAlgorithm::CYKAlgorithm()
{
}

CYKAlgorithm::~CYKAlgorithm()
{
}
