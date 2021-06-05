#include "GrammarEditor.hpp"

#include <functional>
#include <iostream>
#include <memory>

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include "CYKVisualisationTab.hpp"
#include "STVisualisationTab.hpp"

FORCE_LINK_ME(GrammarEditor);

wxIMPLEMENT_DYNAMIC_CLASS(GrammarEditor, wxPanel);

BEGIN_EVENT_TABLE(GrammarEditor, wxPanel)
EVT_WINDOW_CREATE(GrammarEditor::on_create)
EVT_TEXT_ENTER(wxID_ANY, GrammarEditor::on_change)
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

  sizer->Add(panel, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5));
  SetSizer(sizer);
  sizer->Layout();
}

void GrammarEditor::on_change(wxCommandEvent& evt)
{
  load_visualisation_tabs();

  if (auto* word_input = dynamic_cast<wxTextCtrl*>(evt.GetEventObject());
      word_input)
  {
    auto word = static_cast<std::string>(word_input->GetValue());

    // FIXME: Grammar is hard coded for now
    auto grammar = FormalGrammar{
        .rules = {
         Production{Nonterminal{"S"},
                    {new Nonterminal{"B"}, new Nonterminal{"A"}}},
          Production{Nonterminal{"A"}, {new Terminal{"a", "a"}}},
         Production{Nonterminal{"B"}, {new Terminal{"b", "b"}}},
         Production{Nonterminal{"C"}, {new Terminal{"c", "c"}}},
         Production{Nonterminal{"A"},
                    {new Nonterminal{"C"}, new Nonterminal{"B"}}},
         Production{Nonterminal{"B"},
                    {new Nonterminal{"C"}, new Nonterminal{"B"}}},
         Production{Nonterminal{"C"},
                    {new Nonterminal{"A"}, new Nonterminal{"C"}}}},
        .start = Nonterminal{"S", true}};

    // S -> BA
    // A -> a
    // B -> b
    // C -> c
    // A -> CB
    // B -> CB
    // C -> AC

    for (auto* tab : m_visualisation_tabs)
      tab->update_input(grammar, word);
  }
}

void GrammarEditor::load_visualisation_tabs()
{
  m_visualisation_tabs.resize(2, nullptr);

  if (std::all_of(m_visualisation_tabs.begin(), m_visualisation_tabs.end(),
                  [](auto* tab) { return tab; }))
    return;

  if (auto* cyk =
          dynamic_cast<CYKVisualisationTab*>(FindWindowByName("cyk_tab"));
      cyk)
    m_visualisation_tabs[0] = cyk;

  if (auto* st = dynamic_cast<STVisualisationTab*>(FindWindowByName("st_tab"));
      st)
    m_visualisation_tabs[1] = st;
}
