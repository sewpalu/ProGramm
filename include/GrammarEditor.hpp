#ifndef GRAMMAR_EDITOR_HPP
#define GRAMMAR_EDITOR_HPP

#include <vector>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "VisualisationTab.hpp"

class GrammarEditor : public wxPanel
{
private:
  std::vector<VisualisationTab*> m_visualisation_tabs;

public:
  GrammarEditor();

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_change(wxCommandEvent& evt);

  void load_visualisation_tabs();

  wxDECLARE_DYNAMIC_CLASS(GrammarEditor);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef GRAMMAR_EDITOR_HPP */
