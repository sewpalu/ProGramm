#include "VisualisationWidget.hpp"

#include "wx/dcbuffer.h"
#include "wx/html/forcelnk.h"

#include "TreeRenderer.hpp"

FORCE_LINK_ME(VisualisationWidget);

wxIMPLEMENT_DYNAMIC_CLASS(VisualisationWidget, wxPanel);

BEGIN_EVENT_TABLE(VisualisationWidget, wxPanel)
EVT_PAINT(VisualisationWidget::on_paint)
END_EVENT_TABLE()

VisualisationWidget::VisualisationWidget()
{
  auto tree =
      VisualisationWidget::Tree{.children = {{.text = "leaf"},
                                             {.children = {{.text = "leaf_a"},
                                                           {.text = "leaf_b"},
                                                           {.text = "leaf_c"}},
                                              .text = "node"}},
                                .text = "root"};
  draw_tree(tree);

  SetBackgroundColour(*wxGREEN);

  Show();
}

void VisualisationWidget::draw_tree(const Tree& tree)
{
  m_dynamic_paint = [this, tree]() {
    auto dc = wxBufferedPaintDC{this};
    TreeRenderer{tree, dc, GetSize()}();
    Refresh();
  };
}

void VisualisationWidget::draw_table(const Table& table)
{
  // TODO
}

void VisualisationWidget::on_paint(wxPaintEvent&)
{
  if (m_dynamic_paint)
    m_dynamic_paint();
}
