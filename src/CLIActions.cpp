#include "CLIActions.hpp"

#include <algorithm>
#include <iostream>
#include <memory>

#include "TextVisualisationVisitor.hpp"
#include "CYKAlgorithm.hpp"
#include "DummyGrammarParser.hpp"

void CLIActions::visualiseProduction(const std::string& word)
{
  std::cout << "Syntax productions" << std::endl;

  m_engine.setGrammarParser(std::make_unique<DummyGrammarParser>());

  auto cyk = CYKAlgorithm{};
  const auto syntax_trees = m_engine.parseWord(cyk, word);
  const auto& visualiser = cyk.visualiser();

  auto visualisation = std::make_unique<TextVisualisationVisitor>();
  visualiser.accept(*visualisation);

  std::cout << "---" << std::endl;
  std::cout << visualisation->toString() << std::endl;

  //for (const auto& syntax_tree : m_engine.parseWord({}, word))
  //{
  //  auto visualisation = std::make_unique<TextVisualisationVisitor>();
  //  syntax_tree.visualiser().accept(*visualisation);

  //  std::cout << "---" << std::endl;
  //  std::cout << visualisation->toString() << std::endl;
  //}
}
