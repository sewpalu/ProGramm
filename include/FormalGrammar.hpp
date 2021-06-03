#pragma once

#include <string>
#include <vector>

#include "Nonterminal.hpp"
#include "Production.hpp"

/**
 * Struct representing a formal grammar
 *
 * Using struct and public members, since there's no invariant to keep,
 * i.e. any configurations of `rules` and `start` are in principle allowed.
 * See: http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-struct
 */
struct FormalGrammar
{
  std::vector<Production> rules;
  Nonterminal start;

  /**
   * Checks if the grammar is in CNF.
   *
   * \return  first:  `true` if it's in CNF.
   *          second: The reason(s) why it's not in CNF, if applicable.
   */
  std::pair<bool, std::string> is_cnf() const;

  /**
   * Checks if the grammar is plausible.
   *
   * Currently performed checks
   *  * Are the rules empty
   *  * Does the start symbol produce any rules
   *
   * \return  first:  `true` if it's plausible.
   *          second: The reason(s) why it's not in CNF, if applicable.
   */
  std::pair<bool, std::string> is_plausible() const;
};
