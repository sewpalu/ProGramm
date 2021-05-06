#ifndef VISUALISATION_TAB_HPP
#define VISUALISATION_TAB_HPP

#include <functional>
#include <optional>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Visualisable.hpp"
#include "FormalGrammar.hpp"
#include "Word.hpp"

class StepsDisplay;
class VisualisationWidget;

class VisualisationTab : public wxPanel
{
protected:
  StepsDisplay* m_steps;
  VisualisationWidget* m_visualisation;
  std::optional<Visualisable> m_visualised_thing;

  void visualise();

public:
  virtual ~VisualisationTab() = default;

  virtual void update_input(const FormalGrammar& grammar, const Word& word) = 0;
};

#endif /* ifndef VISUALISATION_TAB_HPP */
