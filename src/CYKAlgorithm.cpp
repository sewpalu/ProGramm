#include "CYKAlgorithm.hpp"
#include <memory>
#include <stdexcept>
#include <system_error>

std::vector<SyntaxTree> CYKAlgorithm::parse(FormalGrammar grammar, Word input)
{
  // if (!input.getSize())
  //  return {};

  m_visualiser = std::make_unique<CYKVisualiser>(input.getSize());
  CYKVisualiser* const cykVisSolution =
      dynamic_cast<CYKVisualiser*>(m_visualiser.get());
  if (!cykVisSolution)
    throw std::runtime_error{"m_visualiser was falsely initialised to "
                             "something that isn't a CYKVisualiser"};

  if (input.content.size() == 0)
  {
    for (size_t rule = 0; rule < grammar.rules.size(); rule++)
    {
      if (grammar.rules.at(rule).rhs().size() == 0)
      {
        if (grammar.rules.at(rule).lhs().getIdentifier() ==
            grammar.start.getIdentifier())
        {
          std::cout << "Wort enthalten!\n";
          cykVisSolution->saveStep();
          return {SyntaxTree(STNode(
              std::make_unique<Nonterminal>(grammar.rules.at(rule).lhs())))};
        }
      }
    }
    cykVisSolution->error = "Empty word cannot be produced by the grammar";
    std::cout << "Wort nicht enthalten!\n";
    return {};
  }

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
      // for first round of CYK only rules with length are relevant, as it's
      // looking for rules that produce exactly one Terminal ("letter" of word)
      if (grammar.rules.at(rulePos).rhs().size() == 1)
      {
        // See Symbol (it's known this is a Terminal) has same Terminal in word
        if (grammar.rules.at(rulePos).rhs().at(0)->getIdentifier() ==
            testTerminal.getIdentifier())
        {
          // Generate CYKLink to represent lowermost link in CYK Matrix - This
          // has no productions
          CYKLink bottomCYKLink(Nonterminal(
              grammar.rules.at(rulePos).rhs().at(0)->getIdentifier()));
          // Create the final CYKLink
          CYKLink terminalLink(grammar.rules.at(rulePos).lhs(), {{{rulePos, wordPos}, bottomCYKLink}});
          // Multiple rules are possible (!), if there are multiple rules producing
          // the terminal
          cykVisSolution->matrix.at(0).at(wordPos).push_back(terminalLink);
        }
      }
    }
    cykVisSolution->saveStep();
  }

  // cykVisSolution->dumpAll();

  std::vector<CYKLink> searchSymbolsLeft;
  std::vector<CYKLink> searchSymbolsRight;

  std::vector<CYKLink> tempProductions;
  // CYKLink tempResult;
  std::vector<std::pair<std::pair<unsigned, unsigned int>, CYKLink>>
      tempPositions;

  // std::cout << "Test im Algorithmus: \n\n\n";

  // Loop to go through CYK Lines from bottom to top
  // Go through line of CYK matrix, start in second (1 in vector) row
  for (unsigned int cykLine = 1; cykLine < input.getSize(); cykLine++)
  {
    // Go through columns of CYK matrix
    // With each line up, there's one column less
    for (unsigned int cykCol = 0; cykCol < input.getSize() - cykLine; cykCol++)
    {
      // std::cout << "Checking: (" << cykLine << "/" << cykCol << ") ------\n";

      tempProductions.clear();

      // Iterate through all possible combinations for Nonterminals on the
      // rightside of the possible production rule
      for (unsigned int combination = 0; combination < cykLine; combination++)
      {
        // std::cout << "Checking Combination " << combination << "\n";
        // Clear for each iteration
        searchSymbolsLeft.clear();
        searchSymbolsRight.clear();

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
            searchSymbolsLeft.push_back(cykVisSolution->matrix.at(combination)
                                            .at(cykCol)
                                            .at(nonterminalCounter));
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
            searchSymbolsRight.push_back(
                cykVisSolution->matrix.at(cykLine - combination - 1)
                    .at(cykCol + combination + 1)
                    .at(nonterminalCounter));
          }
        }
        // unsigned int numberOfLeftNonterminals = searchSymbolsLeft.size();
        // unsigned int numberOfRightNonterminals = searchSymbolsRight.size();

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
                 numberOfLeftNonterminal < searchSymbolsLeft.size();
                 numberOfLeftNonterminal++)
            {
              // std::cout << "Comparison left Nonterminal: " <<
              // grammar.rules.at(prodCounter).rhs().at(0).getIdentifier() << "
              // vs. " <<
              // searchSymbolsLeft.at(numberOfLeftNonterminal).getIdentifier()
              // <<
              // "\n";
              if (grammar.rules.at(prodCounter).rhs().at(0)->getIdentifier() ==
                  searchSymbolsLeft.at(numberOfLeftNonterminal)
                      .getRoot()
                      .getIdentifier())
              {
                // std::cout << " - left equal! \n";
                // Loop through possible Nonterminals on right side
                for (unsigned int numberOfRightNonterminal = 0;
                     numberOfRightNonterminal < searchSymbolsRight.size();
                     numberOfRightNonterminal++)
                {
                  if (grammar.rules.at(prodCounter)
                          .rhs()
                          .at(1)
                          ->getIdentifier() ==
                      searchSymbolsRight.at(numberOfRightNonterminal)
                          .getRoot()
                          .getIdentifier())
                  {
                    CYKLink link(grammar.rules.at(prodCounter).lhs());

                    link.addProduction({{combination, cykCol},
                                        cykVisSolution->matrix.at(combination)
                                            .at(cykCol)
                                            .at(numberOfLeftNonterminal)});

                    link.addProduction(
                        {{cykLine - combination - 1, cykCol + combination + 1},
                         cykVisSolution->matrix.at(cykLine - combination - 1)
                             .at(cykCol + combination + 1)
                             .at(numberOfRightNonterminal)});

                    tempProductions.push_back(link);

                    // Output the currently found link that's been added to the
                    // cyk matrix
                    /*std::cout << "'" << link.getRoot().getIdentifier() << "->"
                              << link.getProductions()
                                     .at(0)
                                     .second.getRoot()
                                     .getIdentifier()
                              << link.getProductions()
                                     .at(1)
                                     .second.getRoot()
                                     .getIdentifier()
                              << "', ";*/
                  }
                }
              }
            }
          }
        }
        // std::cout << "\n";
      }
      if (tempProductions.size() > 0)
      {
        cykVisSolution->setResult({cykLine, cykCol}, tempProductions);
        /*for (size_t i = 0; i < tempProductions.size(); i++)
        {
          bool exists = false;
          for (size_t j = 0; j <
        cykVisSolution->matrix.at(cykLine).at(cykCol).size(); j++)
          {
            if
        (cykVisSolution->matrix.at(cykLine).at(cykCol).at(j).equals(tempProductions.at(i)))
            {
              exists = true;
              break;
            }
          }
          if (!exists)
          {
            cykVisSolution->matrix.at(cykLine).at(cykCol).push_back(
                tempProductions.at(i));
          }
        }*/
      }
      if ((cykLine == 1) && (cykCol == 1))
      {
        /*std::cout << "\n Inspection with " << tempProductions.size()
                  << "----------------------------------------\n";
        for (unsigned int i = 0; i < tempProductions.size(); i++)
        {
          std::cout << tempProductions.at(i).getRoot().getIdentifier()
                    << " ::: ";
        }
        std::cout << "\n
        Inspection----------------------------------------\n";*/
      }
      // std::cout << "Post comparison: " << tempProductions.size() << " vs. "
      // << cykVisSolution->matrix.at(cykLine).at(cykCol).size() << "\n";
      if (!(tempProductions.size() ==
            cykVisSolution->matrix.at(cykLine).at(cykCol).size()))
      {
        /*std::cout << "Missverhaeltnis (temp vs. matrix) in Zeile " << cykCol
                  << " und Spalte " << cykCol << "\n";
        for (unsigned int i = 0; i < tempProductions.size(); i++)
        {
          std::cout << tempProductions.at(i).getRoot().getIdentifier() << ", ";
        }*/

        // std::cout << "\n";

        /*for (unsigned int i = 0;
             i < cykVisSolution->matrix.at(cykCol).at(cykLine).size(); i++)
        {
          std::cout << cykVisSolution->matrix.at(cykCol)
                           .at(cykLine)
                           .at(i)
                           .getRoot()
                           .getIdentifier()
                    << ", ";
        }

        std::cout << "\n\n";*/
      }
      // std::cout << "Checked: " << cykLine << " | " << cykCol << "\n";
      // cykVisSolution->dumpAll();
      cykVisSolution->saveStep();
    }
  }

  // cykVisSolution->dumpAll();

  // std::cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n\n\n";

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
            .getIdentifier() == grammar.start.getIdentifier())
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
    std::cout << "Wort nicht enthalten!\n";
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
