#include "VisualisationTab.hpp"

#include "wx/html/forcelnk.h"

#include "GUIVisualisationVisitor.hpp"
#include "VisualisationWidget.hpp"

void VisualisationTab::visualise()
{
  if (!m_visualised_thing)
    return;

  auto visitor = GUIVisualisationVisitor(*m_visualisation);
  m_visualised_thing->visualiser().accept(visitor);
}
