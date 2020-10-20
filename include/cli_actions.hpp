#ifndef CLI_ACTIONS_H
#define CLI_ACTIONS_H

#include "engine_facade.hpp"

class CLIActions
{
private:
  EngineFacade m_engine;

public:
  CLIActions(const EngineFacade& engine) : m_engine(engine)
  {
  }

  void visualiseProduction(const std::string& word);
};

#endif /* CLI_ACTIONS_H */
