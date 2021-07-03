#include "CLIActions.hpp"

#include "CYKAlgorithm.hpp"
#include "EngineFacade.hpp"
#include "GUIApplication.hpp"
#include "SimpleWordParser.hpp"
#include "TextVisualisationVisitor.hpp"
#include "SyntaxTrees.hpp"

void CLIActions::launch_gui(int argc, char** argv) const
{
  GUIApplication::run(argc, argv);
}

void CLIActions::visualise_st(const std::string& word,
                              const FormalGrammar& grammar,
                              std::ostream& os) const
{
  auto engine = EngineFacade{std::make_unique<SimpleWordParser>()};
  engine.setGrammar(grammar);

  auto cyk = CYKAlgorithm{};
  auto visualisable = SyntaxTrees{engine.parseWord(cyk, word)};

  auto visitor = TextVisualisationVisitor{};
  visualisable.visualiser().accept(visitor);

  os << visitor.toString();
}

void CLIActions::visualise_cyk(const std::string& word, const FormalGrammar& grammar,
                               std::ostream& os) const
{
  auto engine = EngineFacade{std::make_unique<SimpleWordParser>()};
  engine.setGrammar(grammar);

  auto cyk = CYKAlgorithm{};
  engine.parseWord(cyk, word);

  auto visitor = TextVisualisationVisitor{};
  cyk.visualiser().accept(visitor);

  os << visitor.toString();
}
