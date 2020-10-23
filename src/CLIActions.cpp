#include "CLIActions.hpp"

#include <iostream>

#include "TextVisualisationVisitor.hpp"

void CLIActions::visualiseProduction(const std::string& word)
{
  std::cout << "Syntax productions" << std::endl;

  m_engine.setGrammarParser({});

  for (const auto& syntax_tree : m_engine.parseWord({}, word))
  {
    auto visualisation = std::make_unique<TextVisualisationVisitor>();
    syntax_tree.visualiser().accept(*visualisation);

    std::cout << "---" << std::endl;
    std::cout << visualisation->toString() << std::endl;
  }
}
