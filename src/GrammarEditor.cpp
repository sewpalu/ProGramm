#include "GrammarEditor.hpp"

#include "wx/html/forcelnk.h"
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

void GrammarEditor::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "grammar_editor_panel");
  /*
  this->m_alpha_display = new AlphabetDisplay(this);
  std::vector<Nonterminal*> nonterminals = {new Nonterminal("A"),
                                            new Nonterminal("B")};
  std::vector<Terminal*> terminals = {new Terminal("a", "a"),
                                      new Terminal("b", "b")};
  this->m_alpha_display->set_alphabet(nonterminals, terminals);

  wxBoxSizer* alpha_sizer = new wxBoxSizer{wxVERTICAL};

  alpha_sizer->Add(this->m_alpha_display, 1, wxEXPAND | wxALL, 5);

  panel->SetSizer(alpha_sizer);*/

  sizer->Add(panel, 1, wxEXPAND | wxALL, 5);

  SetSizer(sizer);
  sizer->Layout();
}
