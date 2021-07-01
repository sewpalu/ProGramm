#ifndef CLI_ACTIONS_H
#define CLI_ACTIONS_H

#include "EngineFacade.hpp"

class CLIActions
{
public:
  void launch_gui(int argc, char** argv) const;

  void visualise_st(const std::string& word, const FormalGrammar& grammar,
                    std::ostream& os) const;
  void visualise_cyk(const std::string& word, const FormalGrammar& grammar,
                     std::ostream& os) const;
};

#endif /* CLI_ACTIONS_H */
