#ifndef MANAGER_TAB_HPP
#define MANAGER_TAB_HPP

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

#include <vector>
#include <optional>
#include <typeinfo>

#include "FormalGrammar.hpp"
#include "Terminal.hpp"

#include "AlphabetManager.hpp"
#include "ProductionManager.hpp"
#include "GrammarOverviewTab.hpp"

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

  void page_changed(wxBookCtrlEvent& evt);

  void on_erase_background(wxEraseEvent& event);

  void saveToFile(std::string file_name);

  std::vector<FormalGrammar> loadedGrammars;

  std::vector<Production> productions;
  std::optional<Nonterminal> start_symbol;

  std::vector<Nonterminal*> nonterminal_alphabet;
  std::vector<Terminal*> terminal_alphabet;

  //Combo box for selecting whether an added symbol is supposed to be a Terminal or Nonterminal
  wxComboBox* symbol_type_selector;
  //Text field for entering the value of a new symbol
  wxTextCtrl* symbol_value_entry;
  //Checkbox to declare start symbol
  wxCheckBox* start_symbol_selector;

  wxListBox* terminal_display;
  wxListBox* nonterminal_display;

  wxSize old_size;

  wxNotebook* grammar_steps;

  AlphabetManager* alpha_manager;
  ProductionManager* prod_manager;
  GrammarOverviewTab* overview_tab;

  wxWindowID grammar_steps_id;

  wxDECLARE_DYNAMIC_CLASS(ManagerTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef MANAGER_TAB_HPP */

