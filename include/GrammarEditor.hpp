#ifndef GRAMMAR_EDITOR_HPP
#define GRAMMAR_EDITOR_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/xrc/xmlres.h"

#include "GUIApplication.hpp"

class GrammarEditor : public wxPanel
{
public:
  GrammarEditor();

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  wxDECLARE_DYNAMIC_CLASS(GrammarEditor);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef GRAMMAR_EDITOR_HPP */
