#include "VisualisationTab.hpp"

#include "wx/html/forcelnk.h"

#include "GUIVisualisationVisitor.hpp"

void VisualisationTab::visualise()
{
  if (!m_visualised_thing)
  {
    draw_empty();
    return;
  }

  auto visitor = GUIVisualisationVisitor(*this);
  m_visualised_thing->visualiser().accept(visitor);
}
