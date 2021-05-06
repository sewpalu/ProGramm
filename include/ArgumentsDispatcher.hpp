#ifndef ARGUMENTS_DISPATCHER_H
#define ARGUMENTS_DISPATCHER_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "docopt.h"

#include "EngineFacade.hpp"
#include "Settings.hpp"

class ArgumentsDispatcher
{
private:
  const std::vector<std::string> m_argv;

  int m_c_argc;
  char** m_c_argv;

  static constexpr auto s_usage = R"(Formal Grammars.

Usage:
  form_lang gui [options]
  form_lang parse (-f <file> | <pattern>) (-g <grammar_file> | <grammar>) [options]

Options:
  -s <name>, --grammar-syntax <name>  Syntax of the grammar definition. One of: "bnf", "fahr" [default: "bnf"].
  -h, --help                          Show this help.

  `parse`
  -g <file>, --grammar-file <file>  Read grammar from file.
  -f <file>, --input-file <file>    Read input word from <file>.)";

public:
  ArgumentsDispatcher(int argc, char** argv);
  void dispatch() const;

private:
  void dispatch_gui(const docopt::Options& args,
                    std::unique_ptr<EngineFacade> engine) const;
  void dispatch_parse(const docopt::Options& args,
                      std::unique_ptr<EngineFacade> engine) const;
};

#endif /* ARGUMENTS_DISPATCHER_H */
