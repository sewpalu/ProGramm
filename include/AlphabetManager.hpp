#ifndef ALPHABET_MANAGER_HPP
#define ALPHABET_MANAGER_HPP

#include <vector>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/button.h"
#include "wx/checkbox.h"
#include "wx/checklst.h"
#include "wx/combobox.h"
#include "wx/dataview.h"
#include "wx/listbox.h"
#include "wx/listctrl.h"
#include "wx/msgdlg.h"
#include "wx/notebook.h"
#include "wx/scrolwin.h"
#include "wx/vlbox.h"
#include "wx/vscroll.h"
#include "wx/wrapsizer.h"

#include "FormalGrammar.hpp"
#include "Terminal.hpp"

class AlphabetManager : public wxScrolledWindow
{
public:
  AlphabetManager();

  std::vector<Terminal*> get_terminal_alphabet();
  std::vector<Nonterminal*> get_nonterminal_alphabet();
  void set_terminal_alphabet(std::vector<Terminal*> terminals);
  void set_nonterminal_alphabet(std::vector<Nonterminal*> nonterminals);

  Nonterminal get_start_symbol();

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxChildFocusEvent& evt);

  void on_refresh(wxPaintEvent& evt);

  std::vector<FormalGrammar> loadedGrammars;

  std::vector<Nonterminal*> nonterminal_alphabet;
  std::vector<Terminal*> terminal_alphabet;

  void add_symbol(wxCommandEvent& evt);
  void delete_symbol(wxCommandEvent& evt);

  // Combo box for selecting whether an added symbol is supposed to be a
  // Terminal or Nonterminal
  wxComboBox* symbol_type_selector;
  // Text field for entering the value of a new symbol
  wxTextCtrl* symbol_value_entry;
  // Checkbox to declare start symbol
  wxCheckBox* start_symbol_selector;

  // wxListBox* terminal_display;
  wxCheckListBox* nonterminal_display;

  wxCheckListBox* terminal_display;

  wxDECLARE_DYNAMIC_CLASS(AlphabetManager);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef ALPHABET_MANAGER_HPP */
