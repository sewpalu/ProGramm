#include "VisualisationTab.hpp"

#include "wx/html/forcelnk.h"

#include "GUIVisualisationVisitor.hpp"
#include "VisualisationWidget.hpp"

FORCE_LINK(VisualisationWidget);
FORCE_LINK(StepsDisplay);

VisualisationTab::VisualisationTab(Visualisable& visualised_thing)
    : m_visualised_thing(visualised_thing)
{
}

void VisualisationTab::visualise()
{
  auto visitor = GUIVisualisationVisitor(*m_visualisation);
  m_visualised_thing.visualiser().accept(visitor);
}
