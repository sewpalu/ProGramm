#pragma once
#include "WordParser.h"

#include "SyntaxTree.h"
#include "FormalGrammar.h"
#include "Word.h"
#include "CYKVisualiser.h"
#include "CYKLink.h"
#include "Nonterminal.h"
#include "SyntaxTree.h"
#include "STNode.h"
#include "Production.h"


class EarleyParser :
    public WordParser
{
public:
    std::pair<std::vector<SyntaxTree>, CYKVisualiser> parse(FormalGrammar grammar, Word input);

    void setGrammar(FormalGrammar inputGrammar);

    EarleyParser();
    EarleyParser(FormalGrammar inputGrammar);
    ~EarleyParser();

    void dumpMostRecentChart();

private:
    //Probably unnecessary, grammar could be given for each check
    FormalGrammar grammar;

    unsigned int inputPosition;

    Word inputWord;

    //The internal chart of the Earley Parser. Outer vector for each step in processing an input: length(input) + 1
    //For each step, there is a list of productions being processed with two numbers:
    //First: Position of processing int Production
    //Second: Step in which the Production was first predicted
    std::vector<std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>>> chart;

    //Method to scan each position in the input (part 1/3 processing a position using the Earley algorithm)
    //void scan();
    std::vector<Nonterminal> scan();

    //Method to complete each further Production that has been affected by the scanning (part 2/3 processing a position using the Earley algorithm)
    void complete(std::vector<Nonterminal> completedRoots);

    std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>> recurseComplete(Nonterminal completedRoot);

    //Method to predict possible Productions that could be processed in the next step (part 3/3 processing a position using the Earley algorithm)
    void predict();
    void predict(std::string rootIdentifier);
    std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>> recursivePredict(std::string rootIdentifier, std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>> iterationTable);
};