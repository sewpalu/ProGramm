#include "FormalGrammar.hpp"

#include <algorithm>
#include <string>

std::pair<bool, std::string> FormalGrammar::is_cnf() const
{
  std::vector<Production> non_cnf_rules = rules;
  non_cnf_rules.erase(
      std::remove_if(non_cnf_rules.begin(), non_cnf_rules.end(),
                     [](const auto& rule) { return rule.is_cnf(); }),
      non_cnf_rules.end());

  if (non_cnf_rules.empty())
    return {true, {}};

  auto reason = std::string{"The following rule(s) are not in CNF:"};
  for (const auto& rule : non_cnf_rules)
  {
    reason += "\n• " + rule.to_string();
  }
  return {false, reason};
}

std::pair<bool, std::string> FormalGrammar::is_plausible() const
{
  if (rules.empty())
    return {false, "No production rules are specified"};

  if (!any_of(rules.begin(), rules.end(), [this](const auto& rule) {
        return rule.lhs().identifier == start.identifier;
      }))
    return {false, "Start symbol does not cause and productions."};

  return {true, {}};
}
