#include "CYKAlgorithm.hpp"
#include <memory>
#include <stdexcept>
#include <system_error>

std::vector<SyntaxTree> CYKAlgorithm::parse(FormalGrammar grammar, Word input)
{
  if (!input.getSize())
    return {};

  std::pair<unsigned int, unsigned int> position1 = {0, 0};
  std::pair<unsigned int, unsigned int> position2 = {1, 1};

  std::vector<std::pair<unsigned, unsigned int>> testPairs = {position1,
                                                              position2};

  // void setResult(std::pair<unsigned int, unsigned int> position, CYKLink
  // production);

  m_visualiser = std::make_unique<CYKVisualiser>(input.getSize());
  CYKVisualiser* const cykVisSolution =
      dynamic_cast<CYKVisualiser*>(m_visualiser.get());
  if (!cykVisSolution)
    throw std::runtime_error{"m_visualiser was falsely initialised to "
                             "something that isn't a CYKVisualiser"};

  std::cout << "Vorher: " << cykVisSolution->matrix.at(0).at(0).size()
            << "\n\n\n";

  Terminal testTerminal{"", ""};

  // Iterate through Terminals that make up the input word
  for (unsigned int wordPos = 0; wordPos < input.getSize(); wordPos++)
  {
    // Set Terminal to work on in this iteration (wordPos)
    testTerminal = input.at(wordPos);

    // Iterate through rules to see if a rule (with a Nonterminal on the left
    // side) can produce the Terminal
    for (unsigned int rulePos = 0; rulePos < grammar.rules.size(); rulePos++)
    {
      std::vector<std::pair<std::pair<unsigned int, unsigned int>, CYKLink>>
          productions;
      // for first round of CYK only rules with length are relevant, as it's
      // looking for rules that produce exactly one Terminal ("letter" of word)
      if (grammar.rules.at(rulePos).rhs().size() == 1)
      {
        // See Symbol (it's known this is a Terminal) has same Terminal in word
        if (grammar.rules.at(rulePos).rhs().at(0)->identifier ==
            testTerminal.identifier)
        {
          // Generate CYKLink to represent lowermost link in CYK Matrix - This
          // has no productions
          CYKLink bottomCYKLink(Nonterminal(
              grammar.rules.at(rulePos).rhs().at(0)->identifier));
          // Push the info to the productions vector
          productions.push_back({{rulePos, wordPos}, bottomCYKLink});
          // Create the final CYKLink
          CYKLink terminalLink(grammar.rules.at(rulePos).lhs(), productions);
          cykVisSolution->matrix.at(0).at(wordPos).push_back(terminalLink);
        }
      }
    }
    cykVisSolution->saveStep();
  }

  cykVisSolution->dumpAll();

  std::vector<Nonterminal> searchSymbolLeft;
  std::vector<Nonterminal> searchSymbolRight;

  std::vector<CYKLink> tempProductions;
  // CYKLink tempResult;
  std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>>
      tempPositions;

  std::cout << "Test im Algorithmus: \n\n\n";

  // Loop to go through CYK Lines from bottom to top
  // Go through line of CYK matrix, start in second (1 in vector) row
  for (unsigned int cykLine = 1; cykLine < input.getSize(); cykLine++)
  {
    // Go through columns of CYK matrix
    // With each line up, there's one column less
    for (unsigned int cykCol = 0; cykCol < input.getSize() - cykLine; cykCol++)
    {
      std::cout << "Checking: (" << cykLine << "/" << cykCol
                << ") "
                   "-----------------------------------------------------------"
                   "-------------------------------------------------------\n";

      tempProductions.clear();

      // Iterate through all possible combinations for Nonterminals on the
      // rightside of the possible production rule
      for (unsigned int combination = 0; combination < cykLine; combination++)
      {
        // Clear for each iteration
        searchSymbolLeft.clear();
        searchSymbolRight.clear();

        // Loop to get all roots in left cell of CYK Matrix that is being
        // checked
        for (unsigned int nonterminalCounter = 0;
             nonterminalCounter <
             cykVisSolution->matrix.at(combination).at(cykCol).size();
             nonterminalCounter++)
        {
          if (cykVisSolution->matrix.at(combination).at(cykCol).size() >
              nonterminalCounter)
          {
            searchSymbolLeft.push_back(cykVisSolution->matrix.at(combination)
                                           .at(cykCol)
                                           .at(nonterminalCounter)
                                           .getRoot());
          }
        }

        // Loop to get all roots in the right cell of CYK Matrix that is being
        // used
        for (unsigned int nonterminalCounter = 0;
             nonterminalCounter <
             cykVisSolution->matrix.at(cykLine - combination - 1)
                 .at(cykCol + combination + 1)
                 .size();
             nonterminalCounter++)
        {
          if (cykVisSolution->matrix.at(cykLine - combination - 1)
                  .at(cykCol + combination + 1)
                  .size() > nonterminalCounter)
          {
            searchSymbolRight.push_back(
                cykVisSolution->matrix.at(cykLine - combination - 1)
                    .at(cykCol + combination + 1)
                    .at(nonterminalCounter)
                    .getRoot());
          }
        }
        // unsigned int numberOfLeftNonterminals = searchSymbolLeft.size();
        // unsigned int numberOfRightNonterminals = searchSymbolRight.size();

        // Loop through possible productions (max given by number of rules in
        // grammar)
        for (unsigned int prodCounter = 0; prodCounter < grammar.rules.size();
             prodCounter++)
        {
          // If the rhs has a length <2 (=1) that means it has already been
          // handled above
          if ((grammar.rules.at(prodCounter).rhs().size() == 2))
          {
            // Loop through possible Nonterminals on left side
            for (unsigned int numberOfLeftNonterminal = 0;
                 numberOfLeftNonterminal < searchSymbolLeft.size();
                 numberOfLeftNonterminal++)
            {
              // std::cout << "Comparison left Nonterminal: " <<
              // grammar.rules.at(prodCounter).rhs().at(0).identifier << "
              // vs. " <<
              // searchSymbolLeft.at(numberOfLeftNonterminal).identifier <<
              // "\n";
              if (grammar.rules.at(prodCounter).rhs().at(0)->identifier ==
                  searchSymbolLeft.at(numberOfLeftNonterminal).identifier)
              {
                // std::cout << " - left equal! \n";
                // Loop through possible Nonterminals on right side
                for (unsigned int numberOfRightNonterminal = 0;
                     numberOfRightNonterminal < searchSymbolRight.size();
                     numberOfRightNonterminal++)
                {
                  // std::cout << "Comparison right Nonterminal: " <<
                  // grammar.rules.at(prodCounter).rhs().at(1).identifier <<
                  // " vs. " <<
                  // searchSymbolRight.at(numberOfRightNonterminal).identifier
                  // << "\n";
                  if (grammar.rules.at(prodCounter)
                          .rhs().at(1)
                          ->identifier ==
                      searchSymbolRight.at(numberOfRightNonterminal)
                          .identifier)
                  {
                    // std::cout << "Inside: \n";
                    // std::cout <<
                    // cykVisSolution.matrix.at(cykLine).at(cykCol).size() <<
                    // "\n"; std::wcout << " - right equal! \n";
                    CYKLink tempResult(grammar.rules.at(prodCounter).lhs());
                    // if (tempResult.getRoot().identifier == "") std::cout
                    // << "Root Alarm IIIIIIIIIIIIIIIIIIIIIIIIIIIII"; std::cout
                    // << tempResult.getRoot().identifier; Get the exact
                    // CYKLink(s) being referenced in the CYK Matrix
                    
                    std::vector<CYKLink> links;

                    for (size_t i = 0;
                         i < cykVisSolution->matrix.at(combination).at(cykCol).size();
                         i++)
                    {
                      if (grammar.rules.at(prodCounter).rhs().at(0)->identifier ==
                          cykVisSolution->matrix.at(combination)
                              .at(cykCol)
                              .at(i).getRoot().identifier)
                      {
                        links.push_back(cykVisSolution->matrix.at(combination)
                                            .at(cykCol)
                                            .at(i));
                      }
                    }

                    for (size_t i = 0; i < cykVisSolution->matrix
                                               .at(cykLine - combination - 1)
                                               .at(cykCol + combination + 1).size();
                         i++)
                    {
                      if (grammar.rules.at(prodCounter)
                              .rhs().at(1)
                              ->identifier ==
                          cykVisSolution->matrix.at(cykLine - combination - 1)
                              .at(cykCol + combination + 1)
                              .at(i).getRoot().identifier)
                      {
                        links.push_back(
                            cykVisSolution->matrix.at(cykLine - combination - 1)
                                .at(cykCol + combination + 1)
                                .at(i));
                      }
                    }

                    if (!(links.at(0).getRoot().identifier == ""))
                      tempResult.addProduction(
                          {{combination, cykCol}, links.at(0)});
                    if (!(links.at(1).getRoot().identifier == ""))
                      tempResult.addProduction({{cykLine - combination - 1,
                                                 cykCol + combination + 1},
                                                links.at(1)});
                    if (tempResult.getProductions().size() > 0)
                      tempProductions.push_back(tempResult);
                  }
                }
              }
            }
          }
        }
      }
      if (tempProductions.size() > 0)
      {
        cykVisSolution->setResult({cykLine, cykCol}, tempProductions);
      }
      if ((cykLine == 1) && (cykCol == 1))
      {
        std::cout << "\n Inspection with " << tempProductions.size()
                  << "----------------------------------------\n";
        for (unsigned int i = 0; i < tempProductions.size(); i++)
        {
          std::cout << tempProductions.at(i).getRoot().identifier
                    << " ::: ";
        }
        std::cout << "\n Inspection----------------------------------------\n";
      }
      // std::cout << "Post comparison: " << tempProductions.size() << " vs. "
      // << cykVisSolution->matrix.at(cykLine).at(cykCol).size() << "\n";
      if (!(tempProductions.size() ==
            cykVisSolution->matrix.at(cykLine).at(cykCol).size()))
      {
        std::cout << "Missverhaeltnis (temp vs. matrix) in Zeile " << cykCol
                  << " und Spalte " << cykCol << "\n";
        for (unsigned int i = 0; i < tempProductions.size(); i++)
        {
          std::cout << tempProductions.at(i).getRoot().identifier << ", ";
        }

        std::cout << "\n";

        for (unsigned int i = 0;
             i < cykVisSolution->matrix.at(cykCol).at(cykLine).size(); i++)
        {
          std::cout << cykVisSolution->matrix.at(cykCol)
                           .at(cykLine)
                           .at(i)
                           .getRoot()
                           .identifier
                    << ", ";
        }

        std::cout << "\n\n";
      }
      cykVisSolution->saveStep();
    }
  }

  std::cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n\n\n";

  bool included = false;

  for (unsigned int nonterminalCounter = 0;
       nonterminalCounter <
       cykVisSolution->matrix.at(input.getSize() - 1).at(0).size();
       nonterminalCounter++)
  {
    if (cykVisSolution->matrix.at(input.getSize() - 1)
            .at(0)
            .at(nonterminalCounter)
            .getRoot()
            .identifier == grammar.start.identifier)
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
    cykVisSolution->error = "Word is not in grammar";
  }

  cykVisSolution->success = included;
  return cykVisSolution->convertToSyntaxTrees(grammar);
}

CYKAlgorithm::CYKAlgorithm() : WordParser(std::make_unique<CYKVisualiser>())
{
}

CYKAlgorithm::~CYKAlgorithm()
{
}
