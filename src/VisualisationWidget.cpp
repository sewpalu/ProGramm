#include "VisualisationWidget.hpp"

#include "wx/html/forcelnk.h"

FORCE_LINK_ME(VisualisationWidget);

wxIMPLEMENT_DYNAMIC_CLASS(VisualisationWidget, wxPanel);

VisualisationWidget::VisualisationWidget()
{
  Show();
}

void VisualisationWidget::draw_tree(const Tree& tree)
{
  // TODO
}

void VisualisationWidget::draw_table(const Table& table)
{
  // TODO
}

