#include "ArgumentsDispatcher.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream> //Necessary on windows
#include <memory>

#include "CLIActions.hpp"
#include "GrammarConverter.hpp"
#include "SimpleWordParser.hpp"

#include "FormalGrammar.hpp"
#include "GUIApplication.hpp"

namespace
{
std::string load_word_file(const std::filesystem::path& file)
{
  auto stringstr = std::stringstream{};
  stringstr << std::ifstream(file).rdbuf();
  return stringstr.str();
}
}  // namespace

ArgumentsDispatcher::ArgumentsDispatcher(int argc, char** argv)
    : m_argv([=] {
        auto init = std::vector<std::string>{};
        init.reserve(argc > 0 ? static_cast<std::size_t>(argc) : 0);
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

  for (const auto& [name, value] : args)
    std::cout << name << ": " << value << '\n';

  if (args["gui"].asBool())
    dispatch_gui(args);
  else if (args["parse"].asBool())
    dispatch_parse(args);
}

void ArgumentsDispatcher::dispatch_gui(const docopt::Options& args) const
{
  (void)args;
  CLIActions{}.launch_gui(1, m_c_argv);
}

void ArgumentsDispatcher::dispatch_parse(const docopt::Options& args) const
{
  auto grammar_loader =
      args.at("<grammar_file>")
          ? GrammarConverter{args.at("<grammar_file>").asString()}
          : GrammarConverter{};
  auto [start, nonterminals, terminals, productions] =
      grammar_loader.load_grammar_from_std_file(
          args.at("<grammar>") ? args.at("<grammar>").asString() : "default");
  auto grammar = FormalGrammar{productions, start};

  auto word = args.at("<word>")
                  ? args.at("<word>").asString()
                  : load_word_file(args.at("<word_file>").asString());

  auto outfile =
      args.at("<output_file>") ? args.at("<output_file>").asString() : "-";
  auto ofstream = outfile == "-" ? std::optional<std::ofstream>{}
                                 : std::optional{std::ofstream{outfile}};

  if (args.at("st").asBool())
    CLIActions{}.visualise_st(word, grammar,
                              outfile == "-" ? std::cout : ofstream.value());

  else if (args.at("cyk").asBool())
    CLIActions{}.visualise_cyk(word, grammar,
                               outfile == "-" ? std::cout : ofstream.value());
}
