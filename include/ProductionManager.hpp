#ifndef PRODUCTION_MANAGER_HPP
#define PRODUCTION_MANAGER_HPP

#include <vector>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/button.h"
#include "wx/msgdlg.h"
#include "wx/combobox.h"
#include "wx/checkbox.h"
#include "wx/listctrl.h"
#include "wx/listbox.h"
#include "wx/notebook.h"
#include "wx/sizer.h"
#include "wx/wrapsizer.h"
#include "wx/scrolwin.h"
#include "wx/dataview.h"
#include "wx/checklst.h"
#include "wx/vscroll.h"
#include "wx/vlbox.h"
#include "wx/dcbuffer.h"
#include "wx/spinctrl.h"

#include "ConfigLoader.hpp"

#include "FormalGrammar.hpp"
#include "Terminal.hpp"

class ProductionManager : public wxScrolledWindow
{
public:
  ProductionManager();

  void set_terminal_alphabet(std::vector<Terminal*> terminal_alphabet);
  void set_nonterminal_alphabet(std::vector<Nonterminal*> nonterminal_alphabet);
  void set_productions(std::vector<Production> productions);

  std::vector<Production> get_productions();

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxChildFocusEvent& evt);

  void on_rhs_change(wxSpinEvent& evt);

  std::vector<FormalGrammar> loadedGrammars;

  ConfigLoader loader = ConfigLoader();

  std::vector<Nonterminal*> nonterminal_alphabet;
  std::vector<Terminal*> terminal_alphabet;

  void add_production(wxCommandEvent& evt);
  void delete_production(wxCommandEvent& evt);
  
  wxCheckListBox* production_display;

  // Utilities for entering a production
  wxComboBox* lhs_selector;
  wxPanel* production_arrow;
  std::vector<wxComboBox*> rhs_selectors;

  std::vector<Production> productions;

  wxSpinCtrl* number_of_rhs_symbols_selector;

  wxSizer* rhs_sizer;
  wxWindow* rhs_container;

  void update_controls();
  void update_symbol_selectors();

  void draw_arrow(wxBufferedPaintDC* dc);

  wxDECLARE_DYNAMIC_CLASS(ProductionManager);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef PRODUCTION_MANAGER_HPP */
