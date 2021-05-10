#ifndef VISUALISATION_WIDGET_HPP
#define VISUALISATION_WIDGET_HPP

#include <map>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/grid.h"

#include "GUIVisualisationInterface.hpp"

class VisualisationWidget : public wxPanel, public GUIVisualisationInterface
{
private:
  std::function<void()> m_dynamic_paint = {};
  wxGrid* m_table = {};
  std::map<Coord, Callback> m_cell_click_handlers = {};

public:
  VisualisationWidget();

  void draw_tree(const Tree& tree) final;
  void draw_table(const Table& table) final;
  void draw_empty() final;

private:

  void on_paint(wxPaintEvent& evt);
  void on_grid_click(wxGridEvent& evt);

  void generate_grid(const Table& table);

  void reset();

  wxDECLARE_DYNAMIC_CLASS(VisualisationWidget);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef VISUALISATION_WIDGET_HPP */
