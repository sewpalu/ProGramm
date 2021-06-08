#include "VisualisationTab.hpp"

#include "wx/colour.h"
#include "wx/html/forcelnk.h"
#include "wx/notebook.h"

#include "GUIVisualisationVisitor.hpp"

void VisualisationTab::update_input(const FormalGrammar& grammar,
                                    const std::string& word)
{

  m_current_grammar = grammar;
  m_current_word = word;

  if (is_selected())
    render_input();
}

void VisualisationTab::visualise()
{
  if (!m_visualised_thing)
  {
    draw_empty();
    return;
  }

  auto visitor = GUIVisualisationVisitor(*this);
  m_visualised_thing->visualiser().accept(visitor);

  Layout();
}

void VisualisationTab::update_visualisation()
{
  if (!is_selected())
    return;

  render_input();

  if (m_visualised_thing)
    visualise();
  else
    draw_empty();
}

void VisualisationTab::show_diagnostics(const std::string& msg,
                                        DiagnosticsLevel level)
{
  if (!m_diagnostics)
    return;

  m_diagnostics->Show();
  m_diagnostics->SetLabelMarkup(wxString::FromUTF8(msg));
  switch (level)
  {
  case DiagnosticsLevel::info:
    m_diagnostics->SetBackgroundColour(
        m_diagnostics->GetDefaultAttributes().colBg);
    m_diagnostics->SetForegroundColour(
        m_diagnostics->GetDefaultAttributes().colFg);
    break;

  case DiagnosticsLevel::warn:
    m_diagnostics->SetBackgroundColour(*wxYELLOW);
    m_diagnostics->SetForegroundColour(*wxBLACK);
    break;
  case DiagnosticsLevel::error:
    m_diagnostics->SetBackgroundColour(*wxRED);
    m_diagnostics->SetForegroundColour(*wxBLACK);
    break;
  }
  Layout();
}

void VisualisationTab::clear_diagnostics()
{
  if (!m_diagnostics)
    return;
  m_diagnostics->Show(false);
  m_diagnostics->SetLabel({});
  Layout();
}

bool VisualisationTab::is_selected() const
{
  auto notebook = dynamic_cast<wxNotebook*>(GetParent());
  if (!notebook)
  {
    std::cerr << "Visualisation tab is not a direct child of the notebook\n";
    return false;
  }

  return this == notebook->GetCurrentPage();
}
