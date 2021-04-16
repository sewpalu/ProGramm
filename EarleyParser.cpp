#include "EarleyParser.h"

std::pair<std::vector<SyntaxTree>, CYKVisualiser> EarleyParser::parse(FormalGrammar grammar, Word input)
{
    std::cout << "Parsing...\n";

    this->inputPosition = 0;

    this->inputWord = input;

    Nonterminal earleyStart("P", true);

    //Add designated start rule
    for (unsigned int i = 0; i < grammar.rules.size(); i++)
    {
        if (grammar.rules.at(i).lhs.isStartSymbol())
        {
            grammar.rules.at(i).lhs.setStartSymbol(false);
            Production startRule;
            startRule.lhs = earleyStart;
            startRule.rhs.push_back(grammar.rules.at(i).lhs);
            grammar.rules.push_back(startRule);
            break;
        }
    }

    this->grammar = grammar;


    predict(earleyStart.getIdentifier());

    dumpMostRecentChart();

    std::vector<Nonterminal> completedRoots = scan();

    dumpMostRecentChart();

    complete(completedRoots);

    /*for (unsigned int i = 0; i < input.getSize(); i++)
    {
        scan();
        complete();
        //Not necessary on last step, but check with if is not efficient.
        predict();
    }*/

    return std::pair<std::vector<SyntaxTree>, CYKVisualiser>();
}

void EarleyParser::setGrammar(FormalGrammar inputGrammar)
{
    this->grammar = inputGrammar;
}

EarleyParser::EarleyParser()
{
}

EarleyParser::EarleyParser(FormalGrammar inputGrammar)
{
    this->grammar = inputGrammar;
}

EarleyParser::~EarleyParser()
{
}

void EarleyParser::dumpMostRecentChart()
{
    if (this->chart.size() > 0)
    {
        std::cout << "Table Nr. " << this->chart.size() - 1 << "\n";
        for (unsigned int i = 0; i < this->chart.at(this->chart.size() - 1).size(); i++)
        {
            std::cout << this->chart.at(this->chart.size() - 1).at(i).first.lhs.getIdentifier() << " -> ";
            for (unsigned int j = 0; j < this->chart.at(this->chart.size() - 1).at(i).first.rhs.size(); j++)
            {
                if (this->chart.at(this->chart.size() - 1).at(i).second.first == j) std::cout << " * ";
                std::cout << this->chart.at(this->chart.size() - 1).at(i).first.rhs.at(j).getIdentifier();
            }

            if (this->chart.at(this->chart.size() - 1).at(i).first.rhs.size() == this->chart.at(this->chart.size() - 1).at(i).second.first) std::cout << " * ";

            std::cout << " | " << this->chart.at(this->chart.size() - 1).at(i).second.second << "\n";
        }
    }
}

std::vector<Nonterminal> EarleyParser::scan()
{
    std::cout << "Scanning...\n";

    //LHS Nonterminals that are completed by the scan.
    std::vector<Nonterminal> outputVector;

    //Read next Terminal Symbol
    Terminal inputSignal = this->inputWord.at(inputPosition);
    //std::cout << "Scanned: " << inputSignal.getIdentifier() << "\n";
    
    for (unsigned int i = 0; i < this->chart.at(chart.size() - 1).size(); i++)
    {
        //std::cout << this->chart.at(chart.size() - 1).at(i).first.rhs.at(this->chart.at(chart.size() - 1).at(i).second.first).getIdentifier() << "\n";
        if (this->chart.at(chart.size() - 1).at(i).first.rhs.at(this->chart.at(chart.size() - 1).at(i).second.first).getIdentifier() == inputSignal.getIdentifier())
        {
            std::cout << "Scanned: " << inputSignal.value << " - with rule: " << i << "\n";
            //Move processing position forward by one
            this->chart.at(chart.size() - 1).at(i).second.first++;

            if (this->chart.at(chart.size() - 1).at(i).second.first == this->chart.at(chart.size() - 1).at(i).first.rhs.size())
            {
                outputVector.push_back(this->chart.at(chart.size() - 1).at(i).first.lhs);
            }
        }
    }

    //Move input position
    inputPosition++;

    return outputVector;
}

void EarleyParser::complete(std::vector<Nonterminal> completedRoots)
{
    //Check if there are already chart entries
    if (this->chart.size() > 0)
    {
        std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>> currentTempRules = this->chart.at(this->chart.size() - 1);

        //Loop through latest table in Earley chart to check if the next read is among the completed roots
        for (unsigned int i = 0; i < this->chart.at(this->chart.size() - 1).size(); i++)
        {
            //Loop through completed roots to check if one equals next read from Earley chart
            for (unsigned int j = 0; j < completedRoots.size(); j++)
            {
                std::cout << this->chart.at(this->chart.size() - 1).at(i).first.rhs.at(this->chart.at(this->chart.size() - 1).at(i).second.first).getIdentifier() << "\n";
                //Check if next read in rule equals completed Root
                /*if (this->chart.at(this->chart.size() - 1).at(i).first.rhs.at(this->chart.at(this->chart.size() - 1).at(i).second.first).getIdentifier() == completedRoots.at(j).getIdentifier())
                {
                    this->chart.at(this->chart.size() - 1).at(i).second.first++;
                    currentTempRules.push_back(this->chart.at(this->chart.size() - 1).at(i));
                    this->chart.at(this->chart.size() - 1).erase(this->chart.at(this->chart.size() - 1).begin() + i);

                    if (currentTempRules.at(currentTempRules.size() - 1).first.rhs.size() < currentTempRules.at(currentTempRules.size() - 1).second.first)
                    {
                        std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>> tempTempRules = recurseComplete(currentTempRules.at(currentTempRules.size() - 1).first.lhs);
                        for (unsigned int k = 0; k < tempTempRules.size(); k++)
                        {
                            currentTempRules.push_back(tempTempRules.at(k));
                        }
                    }
                }*/
            }
        }

        for (unsigned int i = 0; i < this->chart.at(this->chart.size() - 1).size(); i++)
        {
            currentTempRules.push_back(this->chart.at(this->chart.size() - 1).at(i));
        }
        this->chart.pop_back();
        this->chart.push_back(currentTempRules);
    }
}

std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>> EarleyParser::recurseComplete(Nonterminal completedRoot)
{
    return std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>>();
}

void EarleyParser::predict()
{

}

void EarleyParser::predict(std::string rootIdentifier)
{
    //Table of predicted rules for this iteration
    std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>> iterationTable;

    //Iterate through all rules and add rules with predictRoot as lhs to iterationTable
    for (unsigned int i = 0; i < this->grammar.rules.size(); i++)
    {
        //std::cout << "Checking: " << this->grammar.rules.at(i).lhs.getIdentifier() << " vs. " << rootIdentifier << "\n";
        if (this->grammar.rules.at(i).lhs.getIdentifier() == rootIdentifier)
        {
            //Check if rule already exists
            bool ruleExists = false;
            //Iterate through all existing rules in iterationTable
            for (unsigned int j = 0; j < iterationTable.size(); j++)
            {
                if (this->grammar.rules.at(i).rhs.size() == iterationTable.at(j).first.rhs.size())
                {
                    ruleExists = true;
                    for (unsigned int k = 0; k < this->grammar.rules.at(i).rhs.size(); k++)
                    {
                        if (this->grammar.rules.at(i).rhs.at(k).getIdentifier() != iterationTable.at(j).first.rhs.at(k).getIdentifier())
                        {
                            ruleExists = false;
                            break;
                        }
                    }
                }
            }

            if (!ruleExists)
            {
                std::pair < Production, std::pair<unsigned int, unsigned int>> item;
                item.first = grammar.rules.at(i);

                std::cout << "Predicted: " << item.first.lhs.getIdentifier() << " -> ";
                for (unsigned int j = 0; j < item.first.rhs.size(); j++)
                {
                    std::cout << item.first.rhs.at(j).getIdentifier() << " ";
                }
                std::cout << "\n";

                // Newly predicted rules always start processing at position 0
                item.second.first = 0;
                // Set the chart position from which current prediction was started as starting position for the rules
                item.second.second = this->chart.size();

                iterationTable.push_back(item);

                iterationTable = this->recursivePredict(item.first.rhs.at(0).getIdentifier(), iterationTable);
            }
        }
    }

    //Add the table of predicted rules for this iteration to the Earley chart
    this->chart.push_back(iterationTable);
}

std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>>  EarleyParser::recursivePredict(std::string rootIdentifier, std::vector<std::pair<Production, std::pair<unsigned int, unsigned int>>>  iterationTable)
{
    //std::cout << "Recurse Predict: " << iterationTable.size() << "\n";
    //Iterate through all rules and add rules with predictRoot as lhs to iterationTable
    for (unsigned int i = 0; i < this->grammar.rules.size(); i++)
    {
        /*std::cout << "======== CHECKING: " << rootIdentifier << " vs. " << this->grammar.rules.at(i).lhs.getIdentifier() << " -> ";
        for (unsigned int j = 0; j < this->grammar.rules.at(i).rhs.size(); j++)
        {
            std::cout << this->grammar.rules.at(i).rhs.at(j).getIdentifier();
        }
        std::cout << "\n";*/

        if (this->grammar.rules.at(i).lhs.getIdentifier() == rootIdentifier)
        {
           // std::cout << "Part 1 success!\n";
            //Check if rule already exists
            bool ruleExists = false;
            //Iterate through all existing rules in the iterationTable
            for (unsigned int j = 0; j < iterationTable.size(); j++)
            {
                if (iterationTable.at(j).first.lhs.getIdentifier() == rootIdentifier)
                {
                    //std::cout << "Checked here: " << this->grammar.rules.at(j).lhs.getIdentifier() << " vs. " << rootIdentifier  << "\n";
                    if (this->grammar.rules.at(i).rhs.size() == iterationTable.at(j).first.rhs.size())
                    {
                        ruleExists = true;
                        for (unsigned int k = 0; k < this->grammar.rules.at(i).rhs.size(); k++)
                        {
                            if (this->grammar.rules.at(i).rhs.at(k).getIdentifier() != iterationTable.at(j).first.rhs.at(k).getIdentifier())
                            {
                                ruleExists = false;
                                break;
                            }
                        }
                    }
                }
            }

            //std::cout << "Part 2 status: " << std::to_string(ruleExists) << "\n";

            if (!ruleExists)
            {
                //td::cout << "Part 2 success!\n";
                std::pair < Production, std::pair<unsigned int, unsigned int>> item;
                item.first = grammar.rules.at(i);

                std::cout << "Predicted: " << item.first.lhs.getIdentifier() << " -> ";
                for (unsigned int j = 0; j < item.first.rhs.size(); j++)
                {
                    std::cout << item.first.rhs.at(j).getIdentifier() << " ";
                }
                std::cout << "\n";

                // Newly predicted rules always start processing at position 0
                item.second.first = 0;
                // Set the chart position from which current prediction was started as starting position for the rules
                item.second.second = this->chart.size();

                iterationTable.push_back(item);

                iterationTable = this->recursivePredict(item.first.rhs.at(0).getIdentifier(), iterationTable);
            }
        }
    }

    //Add the table of predicted rules for this iteration to the Earley chart
    //this->chart.push_back(iterationTable);

    return iterationTable;
}
