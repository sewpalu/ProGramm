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

class GrammarOverviewTab : public wxScrolledWindow
{
public:
  GrammarOverviewTab();
  GrammarOverviewTab(wxWindow* parent, wxWindowID id);

  void set_productions(std::vector<Production> productions);
  void set_nonterminal_alphabet(std::vector<Nonterminal*> nonterminals);
  void set_terminal_alphabet(std::vector<Terminal*> terminals);
  //void set_start_symbol(Nonterminal start_symbol);

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_refresh(wxPaintEvent& evt);

  void save_grammar(wxCommandEvent& evt);

  GrammarConverter converter;

  std::vector<Terminal*> terminal_alphabet;
  std::vector<Nonterminal*> nonterminal_alphabet;
  std::vector<Production> productions;

  wxListBox* terminal_display;
  wxListBox* nonterminal_display;
  wxListBox* production_display;

  wxDECLARE_DYNAMIC_CLASS(GrammarOverviewTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef GRAMMAR_OVERVIEW_HPP */
