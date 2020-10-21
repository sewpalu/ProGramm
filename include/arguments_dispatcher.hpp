#ifndef ARGUMENTS_DISPATCHER_H
#define ARGUMENTS_DISPATCHER_H

#include <string>
#include <vector>

#include "settings.hpp"

class ArgumentsDispatcher
{
private:
  Settings m_settings;
  std::vector<std::string> m_argv;

public:
  ArgumentsDispatcher(int argc, char** argv);
  void dispatch();

private:
  void print_help(const std::string& context);
};

#endif /* ARGUMENTS_DISPATCHER_H */