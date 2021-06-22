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

  std::vector<std::function<void(GUIVisualisationInterface&)>> m_option_handlers;

  wxSlider* m_zoom_slider;
  wxPanel* m_visualisation_panel;
  wxListBox* m_options_display;
  wxStaticText* m_options_display_label;

public:
  STVisualisationTab();

  void draw_tree(const SyntaxTree& tree) final;

  void draw_empty() final;

  void set_options(const std::vector<Option>& options) final;

private:
  void on_paint(wxPaintEvent& evt);
  void mouseMoved(wxMouseEvent& evt);
  void sliderMoved(wxScrollEvent& evt);
  void on_resize(wxSizeEvent& evt);
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
