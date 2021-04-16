#include "CYKVisualiser.h"
#include "Production.h"
#include "Nonterminal.h"
#include "Terminal.h"
#include "Symbol.h"
#include "Word.h"
#include "FormalGrammar.h"
#include "CYKAlgorithm.h"
#include "SyntaxTree.h"

#include <iostream>
#include <vector>
#include <chrono>


typedef std::chrono::high_resolution_clock Clock;

void main()
{
	//Test 1
	/*//Define Nonterminals for testing
	Nonterminal nonS("S");
	Nonterminal nonA("A");
	Nonterminal nonB("B");

	//Define Terminals for testing
	Terminal a("termA", "a");
	Terminal b("termB", "b");

	//Combine the right hand sides of the production rules into a vector
	//Example the righthand side of the rule "A -> AB" is "AB" which is represented in a vector with the two items Nonterminal A and Nonterminal B
	std::vector<Symbol> rhs1 = {nonA, nonB};
	std::vector<Symbol> rhs2 = {nonA, nonB};
	std::vector<Symbol> rhs3 = {a};
	std::vector<Symbol> rhs4 = {nonB, nonA};
	std::vector<Symbol> rhs5 = {b};

	//Combine the nonterminals (left hand side) and right hand side of the production rules
	Production prod1(nonS, rhs1);
	Production prod2(nonA, rhs2);
	Production prod3(nonA, rhs3);
	Production prod4(nonB, rhs4);
	Production prod5(nonB, rhs5);

	//Combine production rules and start symbol to grammar
	std::vector<Production> rules = {prod1, prod2, prod3, prod4, prod5};
	FormalGrammar testGrammar(nonS, rules);

	//create test word
	std::vector<Terminal> testWordContent = { a,b,b,a };
	Word testWord(testWordContent);*/

	//Test2

	//Define Nonterminals for testing
	Nonterminal nonS("S");
	Nonterminal nonA("A");
	Nonterminal nonB("B");
	Nonterminal nonC("C");

	//Define Terminals for Testing
	Terminal a("termA", "a");
	Terminal b("termB", "b");
	Terminal c("termC", "c");

	//Define right hand sides of production rules
	std::vector<Symbol> rhs1 = {nonA, nonB};
	std::vector<Symbol> rhs2 = {nonB, nonC};
	std::vector<Symbol> rhs3 = {nonB, nonC};
	std::vector<Symbol> rhs4 = {nonC, nonA};
	std::vector<Symbol> rhs5 = {a};
	std::vector<Symbol> rhs6 = {b};
	std::vector<Symbol> rhs7 = {c};

	//Combine Nonterminals and right hand side vectors to production rules
	Production prod1(nonS, rhs1);
	Production prod2(nonA, rhs2);
	Production prod3(nonB, rhs3);
	Production prod4(nonC, rhs4);
	Production prod5(nonA, rhs5);
	Production prod6(nonB, rhs6);
	Production prod7(nonC, rhs7);

	//Combine production rules to vector
	std::vector<Production> productionRules = {prod1, prod2, prod3, prod4, prod5, prod6, prod7};
	
	//Combine to grammar
	FormalGrammar testGrammar(nonS, productionRules);

	//Create test word
	std::vector<Terminal> testWordContent = { b, c, b, c, a };
	Word testWord(testWordContent);



	//Create CYKAlgorithm object
	CYKAlgorithm cykTest;

	//execute parse of CYKAlgorithm and take time
	auto start_time = Clock::now();
	CYKVisualiser cykVis = cykTest.parse(testGrammar, testWord);
	auto end_time = Clock::now();
	std::cout << "Laufzeit: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "Mikrosekunden" << std::endl;

	std::string user_input;

	std::string instructions = "Please type 'quit' to quit this application.\n";
	instructions = instructions + "Please type 'details' to view details for a production in the CYK matrix.\n";

	while (true)
	{
		std::cout << instructions;

		std::cin >> user_input;
		if (user_input == "quit")
		{
			break;
		}
		else if (user_input == "details")
		{
			try {
				std::cout << "Please type the first coordinate (row) for the field you want to inspect.\n";
				unsigned int firstCoordinate;
				std::cin >> firstCoordinate;
				std::cout << "Please type the second coordinate (column) for the field you want to inspect.\n";
				unsigned int secondCoordinate;
				std::cin >> secondCoordinate;
				std::cout << "Please type the identifier of the Nonterminal in the field that you want to inspect.\n";
				std::string nonterminalIdentifier;
				std::cin >> nonterminalIdentifier;

				cykVis.dumpContent(firstCoordinate, secondCoordinate, nonterminalIdentifier);

			}
			catch(...)
			{
				std::cout << "You seem to have entered an invalid input! Your action has been canceled!\n";
			}
		}
		else
		{
			std::cout << "Your input '" << user_input << "' is not accepted.\n";
		}
	}

	std::cout << "Finished!";
}