#include "GrammarEditor.hpp"

#include "wx/html/forcelnk.h"
#include "wx/stdpaths.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(GrammarEditor);

wxIMPLEMENT_DYNAMIC_CLASS(GrammarEditor, wxPanel);

BEGIN_EVENT_TABLE(GrammarEditor, wxPanel)
EVT_WINDOW_CREATE(GrammarEditor::on_create)
END_EVENT_TABLE()

GrammarEditor::GrammarEditor()
{
  Show();
}

void GrammarEditor::on_create(wxWindowCreateEvent&)
{
  // Flag to prevent recursively calling the create event handler by creating
  // new children.
  static auto once_flag = true;

  if (once_flag)
  {
    once_flag = false;
    if (!wxXmlResource::Get()->LoadPanel(this, "grammar_editor_panel"))
    {
      std::cerr << "Failure loading grammar_editor panel\n";
      return;
    }
  }
}
