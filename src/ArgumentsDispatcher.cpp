#include "ArgumentsDispatcher.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "CLIActions.hpp"
#include "DummyGrammarParser.hpp"

#include "GUIApplication.hpp"

ArgumentsDispatcher::ArgumentsDispatcher(int argc, char** argv)
    : m_argv([=] {
        auto init = std::vector<std::string>{};
        init.reserve(argc);
        for (auto i = 1; i < argc; ++i)
          init.push_back(std::string{argv[i]});
        return init;
      }()),
    m_c_argc(argc), m_c_argv(argv)
{
}

void ArgumentsDispatcher::dispatch() const
{

  auto args = docopt::docopt(s_usage, m_argv);

  auto engine = std::unique_ptr<EngineFacade>{};

  // TODO Parse common options

  if (args["gui"])
    dispatch_gui(args, std::move(engine));
  else if (args["parse"])
    dispatch_parse(args, std::move(engine));
}

void ArgumentsDispatcher::dispatch_gui(const docopt::Options& args, std::unique_ptr<EngineFacade> engine) const
{
  // TODO
  (void)args;
  (void)engine;
  GUIApplication::run(1, m_c_argv);
}

void ArgumentsDispatcher::dispatch_parse(const docopt::Options& args, std::unique_ptr<EngineFacade> engine) const
{
  // TODO
  (void)args;
  (void)engine;
}
