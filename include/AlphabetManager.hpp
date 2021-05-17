#ifndef ALPHABET_MANAGER_HPP
#define ALPHABET_MANAGER_HPP

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

#include <wx/wrapsizer.h>

#include <wx/scrolwin.h>

#include <wx/dataview.h>

#include <wx/checklst.h>

#include <wx/vscroll.h>

#include <wx/vlbox.h>

#include <vector>

#include "FormalGrammar.hpp"
#include "Terminal.hpp"



class AlphabetManager : public wxScrolledWindow
{
public:
  AlphabetManager();
  AlphabetManager(wxWindow* parent, wxWindowID id);

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxChildFocusEvent& evt);

  void on_refresh(wxPaintEvent& evt);

  void saveToFile(std::string file_name);

  std::vector<FormalGrammar> loadedGrammars;

  std::vector<Nonterminal> nonterminal_alphabet;
  std::vector<Terminal> terminal_alphabet;

  void add_symbol(wxCommandEvent& evt);

  int add_symbol_button_id = 42;

  // Combo box for selecting whether an added symbol is supposed to be a
  // Terminal or Nonterminal
  wxComboBox* symbol_type_selector;
  // Text field for entering the value of a new symbol
  wxTextCtrl* symbol_value_entry;
  // Checkbox to declare start symbol
  wxCheckBox* start_symbol_selector;

  //wxListBox* terminal_display;
  wxCheckListBox* nonterminal_display;

  // Utilities for entering a production
  wxComboBox* lhs_selector;
  std::vector<wxComboBox*> rhs_selectors;

  //wxListCtrl* terminal_display;

  wxCheckListBox* terminal_display;

  wxBoxSizer* sizer;


  wxDECLARE_DYNAMIC_CLASS(AlphabetManager);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef ALPHABET_MANAGER_HPP */