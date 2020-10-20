#include "cli_actions.hpp"

#include <iostream>

#include "text_visualisation_visitor.hpp"

void CLIActions::visualiseProduction(const std::string& word)
{
  std::cout << "Syntax productions" << std::endl;

  for (const auto& syntax_tree : m_engine.parseWord(word))
  {
    auto visualisation = std::make_shared(TextVisualisationVisitor{});
    syntax_tree.accept(visualisation);

    std::cout << "---" << std::endl;
    std::cout << visualisation.to_string() << std::endl;
  }
}
