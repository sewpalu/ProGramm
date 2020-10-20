#include "arguments_dispatcher.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

ArgumentsDispatcher::ArgumentsDispatcher(int argc, char** argv)
{
  m_argv.reserve(argc);
  for (auto i = 0; i < argc; ++i)
    m_argv.push_back(argv[i]);
}

void ArgumentsDispatcher::dispatch()
{
  // Temporary implementation!

  if (m_argv.size() != 2)
  {
    print_help("Invalid number of arguments.");
    return;
  }

  {
    auto grammar_parser = std::unique_ptr<GrammarParser>{nullptr};
    switch (m_settings.syntax)
    {
    case Settings::Syntax::dummy:
      grammar_parser = std::make_unique<GrammarParser>(DummySyntax);
      break;
    case Settings::Syntax::bnf:
      throw std::runtime_error{"BNF not yet implemented!"};
      break;
    case Settings::Syntax::fahr:
      throw std::runtime_error{"Fahr notation not yet implemented!"};
      break;
    }

    CLIActions{EngineFacade{std::move(grammar_parser)}}.visualiseProduction(
        m_argv[1]);
  }
}

void ArgumentsDispatcher::print_help(const std::string& context)
{
  std::cout << "Wrong! " << context << std::endl;
  std::cout << std::endl;
  std::cout << "Expected Usage:" << std::endl;
  std::cout << "    " << m_argv[0] << " <word>" << std::endl;
}
