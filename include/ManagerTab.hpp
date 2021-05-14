#ifndef MANAGER_TAB_HPP
#define MANAGER_TAB_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/button.h"

#include <wx/msgdlg.h>

#include <wx/combobox.h>

#include "FormalGrammar.hpp"
#include "Terminal.hpp"

class ManagerTab : public wxPanel
{
public:
  ManagerTab();

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

  //Combo box for selecting whether an added symbol is supposed to be a Terminal or Nonterminal
  wxComboBox* symbol_type_selector;
  //Text field for entering the value of a new symbol
  wxTextCtrl* symbol_value_entry;

  wxDECLARE_DYNAMIC_CLASS(ManagerTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef MANAGER_TAB_HPP */

