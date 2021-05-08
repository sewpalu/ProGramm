#ifndef VISUALISATION_WIDGET_HPP
#define VISUALISATION_WIDGET_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "GUIVisualisationInterface.hpp"

class VisualisationWidget : public wxPanel, public GUIVisualisationInterface
{
private:
  std::function<void()> m_dynamic_paint = {};

public:
  VisualisationWidget();

  void draw_tree(const Tree& tree) final;
  void draw_table(const Table& table) final;

private:

  void on_paint(wxPaintEvent& evt);

  wxDECLARE_DYNAMIC_CLASS(VisualisationWidget);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef VISUALISATION_WIDGET_HPP */
