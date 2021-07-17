#ifndef CLI_ACTIONS_H
#define CLI_ACTIONS_H

#include "EngineFacade.hpp"

/**
 * Collection of all possible actions that can be launched from the command
 * line.
 *
 * The class is not concerned with interpreting CLI arguments, but rather with
 * processing already interpreted arguments.
 */
class CLIActions
{
public:
  /**
   * Launches the GUI.
   *
   * Needs the application name in the argc, argv format.
   *
   * \param argc  At least 1
   * \param argv  At least something like ["build/pro_gramm"]
   */
  void launch_gui(int argc, char** argv) const;

  /**
   * Textually visualises the ST for a given word and grammar.
   *
   * \param word      The word to parse.
   * \param grammar   The grammar to use.
   * \param os        The resulting text is outputted here.
   */
  void visualise_st(const std::string& word, const FormalGrammar& grammar,
                    std::ostream& os) const;

  /**
   * Textually visualises the CYK Matrix of parsing a given word and grammar.
   *
   * \param word      The word to parse.
   * \param grammar   The grammar to use.
   * \param os        The resulting text is outputted here.
   */
  void visualise_cyk(const std::string& word, const FormalGrammar& grammar,
                     std::ostream& os) const;
};

#endif /* CLI_ACTIONS_H */
