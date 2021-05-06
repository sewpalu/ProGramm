#ifndef VISUALISATION_TAB_HPP
#define VISUALISATION_TAB_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Visualisable.hpp"

class StepsDisplay;
class VisualisationWidget;

class VisualisationTab : public wxPanel
{
protected:
  StepsDisplay* m_steps;
  VisualisationWidget* m_visualisation;
  const Visualisable& m_visualised_thing;

  VisualisationTab(Visualisable& visualised_thing);

  void visualise();

public:
  virtual ~VisualisationTab() = default;
};

#endif /* ifndef VISUALISATION_TAB_HPP */
