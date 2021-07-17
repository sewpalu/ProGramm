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
    : m_grammar({.rules = {}, .start = Nonterminal{""}})
{
  Show();
}

void GrammarEditor::set_grammar(const FormalGrammar& grammar)
{
  m_grammar = grammar;
  m_productions_display->set_productions(m_grammar.rules);
  m_startsymbol_display->SetLabel(
      wxString::FromUTF8(m_grammar.start.identifier));
  notify_visualisation();
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

  m_word_input = dynamic_cast<wxTextCtrl*>(FindWindowByName("side_word_input"));
  if (!m_word_input)
  {
    std::cerr << "Unable to load word input in side panel\n";
    return;
  }

  m_productions_display = dynamic_cast<ProductionDisplay*>(
      FindWindowByName("side_productions_display"));
  if (!m_productions_display)
  {
    std::cerr << "Unable to load productions display in side panel\n";
    return;
  }

  m_startsymbol_display =
      dynamic_cast<wxStaticText*>(FindWindowByName("side_startsymbol_display"));
  if (!m_startsymbol_display)
  {
    std::cerr << "Unable to load start symbol display in side panel\n";
    return;
  }

  m_tabs = dynamic_cast<wxNotebook*>(FindWindowByName("tabs"));
  if (!m_tabs)
  {
    std::cerr << "Unable to load visualisation tabs notebook from side panel\n";
    return;
  }
  m_tabs->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &GrammarEditor::on_change, this);
}

void GrammarEditor::on_change(wxCommandEvent&)
{
  notify_visualisation();
}
void GrammarEditor::notify_visualisation()
{
  load_visualisation_tabs();

  for (auto* tab : m_visualisation_tabs)
    if (tab)
      tab->update_input(m_grammar, m_word_input->GetValue().ToStdString());
}

void GrammarEditor::load_visualisation_tabs()
{
  if (!m_tabs)
  {
    m_tabs = dynamic_cast<wxNotebook*>(FindWindowByName("tabs"));
    if (!m_tabs)
    {
      std::cerr
          << "Unable to load visualisation tabs notebook from side panel\n";
      return;
    }
    m_tabs->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &GrammarEditor::on_change, this);
  }
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
