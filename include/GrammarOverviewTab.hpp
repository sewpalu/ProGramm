#ifndef GRAMMAR_OVERVIEW_HPP
#define GRAMMAR_OVERVIEW_HPP

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
#include "GrammarConverter.hpp"
#include "AlphabetDisplay.hpp"
#include "ProductionDisplay.hpp"

class GrammarOverviewTab : public wxScrolledWindow
{
public:
  GrammarOverviewTab();

  void set_productions(std::vector<Production> productions);
  void set_nonterminal_alphabet(std::vector<Nonterminal*> nonterminals);
  void set_terminal_alphabet(std::vector<Terminal*> terminals);

  std::vector<Production> get_productions();
  std::vector<Nonterminal*> get_nonterminal_alphabet();
  std::vector<Terminal*> get_terminal_alphabet();

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void save_grammar(wxCommandEvent& evt);

  void load_grammar(wxCommandEvent& evt);
  void delete_grammars(wxCommandEvent& evt);
  void check_grammar(wxCommandEvent& evt);

  void update_displays();

  GrammarConverter converter;

  std::vector<Terminal*> terminal_alphabet;
  std::vector<Nonterminal*> nonterminal_alphabet;
  std::vector<Production> productions;

  wxListBox* terminal_display;
  wxListBox* nonterminal_display;
  AlphabetDisplay* m_alpha_display;
  ProductionDisplay* m_prod_display;

  wxListBox* production_display;

  wxTextCtrl* grammar_name_entry;

  wxCheckListBox* grammars_display;

  wxDECLARE_DYNAMIC_CLASS(GrammarOverviewTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef GRAMMAR_OVERVIEW_HPP */
