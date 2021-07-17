#ifndef CYK_VISUALISATION_TAB_HPP
#define CYK_VISUALISATION_TAB_HPP

#include <map>
#include <stdexcept>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/grid.h"
#include "wx/notebook.h"

#include "VisualisationTab.hpp"

/**
 * wxPanel for visualising a CYK Matrix
 *
 * This includes the matrix itself, as well as controls for skipping through
 * steps of its development.
 */
class CYKVisualisationTab : public VisualisationTab
{
private:
  wxGrid* m_table = nullptr;
  wxButton* m_prev_button = nullptr;
  wxButton* m_next_button = nullptr;
  std::map<Coord, Callback> m_cell_click_handlers = {};

public:

  CYKVisualisationTab();

  /// GUIVisualisationInterface implementation

  void draw_table(const Table& table) final;

  void draw_empty() final;

  void set_button(const std::string& label, Callback on_click,
                  Position position = Position::left) final;

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxBookCtrlEvent& evt);
  void on_grid_click(wxGridEvent& evt);
  void on_resize(wxSizeEvent& evt);

  void render_input() final;

  void resize_grid(std::size_t n_cols, std::size_t n_rows);

  wxDECLARE_DYNAMIC_CLASS(CYKVisualisationTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef CYK_VISUALISATION_TAB_HPP */
