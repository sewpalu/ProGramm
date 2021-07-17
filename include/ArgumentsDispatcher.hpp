#ifndef ARGUMENTS_DISPATCHER_H
#define ARGUMENTS_DISPATCHER_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "docopt.h"

#include "EngineFacade.hpp"

/**
 * Class for handling command line arguments.
 *
 * Parses the arguments and executes the corresponding actions.
 */
class ArgumentsDispatcher
{
private:
  const std::vector<std::string> m_argv;

  int m_c_argc;
  char** m_c_argv;

  static constexpr auto s_usage = R"(Formal Grammars.

Usage:
  pro_gramm gui [options]
  pro_gramm parse (-f <word_file> | <word>) (-g <grammar_file> | <grammar>) (st|cyk)
                  [-v text] [-o <output_file>] [options]

Options:
  -h, --help                          Show this help.

  `parse`
  -g <file>, --grammar-file <file>    Read grammar from file.
                                      Currently only JSON files are supported.
                                      For reference: Create and save a grammar with the
                                      GUI and examine the created grammar.json file.
                                      In the file, the grammar named "default" is used.
  -f <file>, --in-file <file>         Read input word from <file>.
  -v <format>, --vis-format <format>  Output format of the visualisation.
                                      Currently only "text" is supported.
                                      [default: "text"]
  -o <file>, --output <file>          File the output is written to.
                                      "-" means standard output. [default: "-"])";

public:
  /**
   * Constructor
   *
   * Supply with the c-style arguments from main, or something that is compatible
   */
  ArgumentsDispatcher(int argc, char** argv);

  /**
   * Executes the actions that correspond to the provided arguments.
   */
  void dispatch() const;

private:
  /**
   * Dispatch for the "gui" cli verb.
   */
  void dispatch_gui(const docopt::Options& args) const;

  /**
   * Dispatch for the "parse" cli verb.
   */
  void dispatch_parse(const docopt::Options& args) const;
};

#endif /* ARGUMENTS_DISPATCHER_H */
