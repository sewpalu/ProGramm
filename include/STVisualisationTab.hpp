#ifndef ST_VISUALISATION_TAB_HPP
#define ST_VISUALISATION_TAB_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/notebook.h"

#include "VisualisationTab.hpp"

#include "SyntaxTree.hpp"

class STVisualisationTab : public VisualisationTab
{
public:
  STVisualisationTab();

  /**
   * Stores the resulting SyntaxTree and updates the visualisation.
   */
  void update_input(const FormalGrammar& grammar, const Word& word) final;

private:
  void mouseMoved(wxMouseEvent& evt);
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxBookCtrlEvent& evt);

  void update_visualisation();

  wxDECLARE_DYNAMIC_CLASS(STVisualisationTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef ST_VISUALISATION_TAB_HPP */
