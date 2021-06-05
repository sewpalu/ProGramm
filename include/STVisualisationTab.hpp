#ifndef ST_VISUALISATION_TAB_HPP
#define ST_VISUALISATION_TAB_HPP

#include <chrono>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/notebook.h"

#include "SyntaxTree.hpp"
#include "TreeRenderer.hpp"
#include "VisualisationTab.hpp"

class STVisualisationTab : public VisualisationTab
{
private:
  std::pair<int, int> m_offset = {0, 0};

  // This is the starting point with a detailed time stamp for dragging the
  // widget
  std::pair<wxPoint, std::chrono::high_resolution_clock::time_point>
      dragStartingPoint;

  std::unique_ptr<SyntaxTree> m_tree = nullptr;

  wxSlider* m_zoom_slider;
  wxPanel* m_visualisation_panel;

public:
  STVisualisationTab();

  void draw_tree(SyntaxTree* tree) final;

  void draw_table(const Table&) final
  {
    throw std::runtime_error(
        "Table drawing not supported by STVisualisationTab");
  }

  void draw_empty() final;

  void add_button(const std::string&, Callback, Position = Position::left) final
  {
    throw std::runtime_error("Buttons not supported by STVisualisationTab");
  }

private:
  void on_paint(wxPaintEvent& evt);
  void mouseMoved(wxMouseEvent& evt);
  void sliderMoved(wxScrollEvent& evt);
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxBookCtrlEvent& evt);

  void render_input() final;

  wxDECLARE_DYNAMIC_CLASS(STVisualisationTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef ST_VISUALISATION_TAB_HPP */
