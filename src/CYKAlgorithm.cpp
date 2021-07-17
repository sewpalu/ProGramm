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
          cykVisSolution->saveStep();
          return {SyntaxTree(STNode(
              std::make_unique<Nonterminal>(grammar.rules.at(rule).lhs())))};
        }
      }
    }
    cykVisSolution->error = "Empty word cannot be produced by the grammar";
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
        if (grammar.rules.at(rulePos).rhs().at(0)->identifier ==
            testTerminal.identifier)
        {
          // Generate CYKLink to represent lowermost link in CYK Matrix - This
          // has no productions
          CYKLink bottomCYKLink(Nonterminal(
              grammar.rules.at(rulePos).rhs().at(0)->identifier));
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

  // Loop to go through CYK Lines from bottom to top
  // Go through line of CYK matrix, start in second (1 in vector) row
  for (unsigned int cykLine = 1; cykLine < input.getSize(); cykLine++)
  {
    // Go through columns of CYK matrix
    // With each line up, there's one column less
    for (unsigned int cykCol = 0; cykCol < input.getSize() - cykLine; cykCol++)
    {
      tempProductions.clear();

      // Iterate through all possible combinations for Nonterminals on the
      // rightside of the possible production rule
      for (unsigned int combination = 0; combination < cykLine; combination++)
      {
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
              if (grammar.rules.at(prodCounter).rhs().at(0)->identifier ==
                  searchSymbolsLeft.at(numberOfLeftNonterminal)
                      .getRoot()
                      .identifier)
              {
                // Loop through possible Nonterminals on right side
                for (unsigned int numberOfRightNonterminal = 0;
                     numberOfRightNonterminal < searchSymbolsRight.size();
                     numberOfRightNonterminal++)
                {
                  if (grammar.rules.at(prodCounter)
                          .rhs()
                          .at(1)
                          ->identifier ==
                      searchSymbolsRight.at(numberOfRightNonterminal)
                          .getRoot()
                          .identifier)
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
      cykVisSolution->saveStep();
    }
  }

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

  if (!included)
  {
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
