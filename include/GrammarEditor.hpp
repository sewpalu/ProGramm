#ifndef GRAMMAR_EDITOR_HPP
#define GRAMMAR_EDITOR_HPP

#include <optional>
#include <vector>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/notebook.h"

#include "FormalGrammar.hpp"
#include "ProductionDisplay.hpp"
#include "VisualisationTab.hpp"

/**
 * wxPanel for displaying the input grammar and allowing entering the input
 * word.
 *
 * Originally intended for editing the grammar as well, hence the name, but that
 * functionality was exported to the ManagerTab.
 */
class GrammarEditor : public wxPanel
{
private:
  std::vector<VisualisationTab*> m_visualisation_tabs;
  FormalGrammar m_grammar;
  wxNotebook* m_tabs;
  wxTextCtrl* m_word_input;
  ProductionDisplay* m_productions_display;
  wxStaticText* m_startsymbol_display;

public:
  GrammarEditor();

  /**
   * Updates the displayed grammar and the visualisations
   */
  void set_grammar(const FormalGrammar& grammar);

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_change(wxCommandEvent& evt);

  /**
   * Updates the visualisations with the currently set input grammar and word.
   */
  void notify_visualisation();

  /**
   * Loads the pointers to the visualisation tabs.
   *
   * Automatically detects, whether they are already loaded.
   * Should be called before accessing the tabs.
   */
  void load_visualisation_tabs();

  wxDECLARE_DYNAMIC_CLASS(GrammarEditor);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef GRAMMAR_EDITOR_HPP */
