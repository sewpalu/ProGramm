#ifndef PRODUCTION_MANAGER_HPP
#define PRODUCTION_MANAGER_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/button.h"

#include <wx/msgdlg.h>

#include <wx/combobox.h>

#include <wx/checkbox.h>

#include <wx/listctrl.h>

#include <wx/listbox.h>

#include <wx/notebook.h>

#include <wx/sizer.h>

#include <wx/wrapsizer.h>

#include <wx/scrolwin.h>

#include <wx/dataview.h>

#include <wx/checklst.h>

#include <wx/vscroll.h>

#include <wx/vlbox.h>

#include <wx/dcbuffer.h>

#include <vector>

#include "FormalGrammar.hpp"
#include "Terminal.hpp"

class ProductionManager : public wxScrolledWindow
{
public:
  ProductionManager();
  ProductionManager(wxWindow* parent, wxWindowID id);

  void set_terminal_alphabet(std::vector<Terminal> terminal_alphabet);
  void set_nonterminal_alphabet(std::vector<Nonterminal> nonterminal_alphabet);

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxChildFocusEvent& evt);

  void on_refresh(wxPaintEvent& evt);

  std::vector<FormalGrammar> loadedGrammars;

  std::vector<Nonterminal> nonterminal_alphabet;
  std::vector<Terminal> terminal_alphabet;

  void add_production(wxCommandEvent& evt);
  
  wxCheckListBox* production_display;

  // Utilities for entering a production
  wxComboBox* lhs_selector;
  wxPanel* production_arrow;
  std::vector<wxComboBox*> rhs_selectors;

  wxWrapSizer* rhs_sizer;

  void update_symbol_selectors();

  void draw_arrow(wxBufferedPaintDC* dc);

  wxBoxSizer* sizer;

  wxDECLARE_DYNAMIC_CLASS(ProductionManager);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef PRODUCTION_MANAGER_HPP */
