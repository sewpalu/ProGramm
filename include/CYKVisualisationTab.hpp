#ifndef CYK_VISUALISATION_TAB_HPP
#define CYK_VISUALISATION_TAB_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "VisualisationTab.hpp"

class CYKVisualisationTab : public VisualisationTab
{
public:
  CYKVisualisationTab();

  /**
   * Stores the resulting CYKVisualiser and updates the visualisation.
   */
  void update_input(const FormalGrammar& grammar, const Word& word) final;

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxChildFocusEvent& evt);

  wxDECLARE_DYNAMIC_CLASS(CYKVisualisationTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef CYK_VISUALISATION_TAB_HPP */
