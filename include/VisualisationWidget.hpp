#ifndef VISUALISATION_WIDGET_HPP
#define VISUALISATION_WIDGET_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "GUIVisualisationInterface.hpp"

class VisualisationWidget : public wxPanel, public GUIVisualisationInterface
{
public:
  VisualisationWidget();

  void draw_tree(const Tree& tree) final;
  void draw_table(const Table& table) final;

private:
  wxDECLARE_DYNAMIC_CLASS(VisualisationWidget);
};

#endif /* ifndef VISUALISATION_WIDGET_HPP */
