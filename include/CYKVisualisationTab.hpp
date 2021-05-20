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

class CYKVisualisationTab : public VisualisationTab
{
private:
  wxGrid* m_table = nullptr;
  std::map<Coord, Callback> m_cell_click_handlers = {};

public:
  CYKVisualisationTab();

  /**
   * Stores the resulting CYKVisualiser and updates the visualisation.
   */
  void update_input(const FormalGrammar& grammar,
                    const std::string& word) final;

  /// GUIVisualisationInterface implementation

  void draw_tree(const Tree& tree) final
  {
    throw std::runtime_error(
        "Tree drawing not supported by CYKVisualisationTab");
  }

  void draw_table(const Table& table) final;

  void draw_empty() final;

  void add_button(const std::string& label, Callback on_click,
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

  void resize_grid(std::size_t n_cols, std::size_t n_rows);

  void update_visualisation();

  wxDECLARE_DYNAMIC_CLASS(CYKVisualisationTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef CYK_VISUALISATION_TAB_HPP */
