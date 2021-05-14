#include "AlphabetManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(AlphabetManager);

wxIMPLEMENT_DYNAMIC_CLASS(AlphabetManager, wxPanel);

BEGIN_EVENT_TABLE(AlphabetManager, wxPanel)
EVT_WINDOW_CREATE(AlphabetManager::on_create)
EVT_CHILD_FOCUS(AlphabetManager::on_page_changed)
// EVT_BUTTON(AlphabetManager::add_symbol)

EVT_PAINT(AlphabetManager::on_refresh)

END_EVENT_TABLE()

AlphabetManager::AlphabetManager(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
  Show();
}

void AlphabetManager::on_page_changed(wxChildFocusEvent& evt)
{
  Layout();
}

void AlphabetManager::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  this->sizer = new wxBoxSizer{wxVERTICAL};

  this->grammar_steps = new wxNotebook(this, wxID_ANY, wxDefaultPosition,
                                       wxSize(this->GetSize()));

  this->alpha_manager = new AlphabetManager(this->grammar_steps, wxID_ANY);

  wxPanel* testWindow = new wxPanel(this->grammar_steps, wxID_ANY);

  testWindow->SetBackgroundColour(wxColor(255, 0, 0));

  // grammar_steps->AddPage(this->alpha_manager, )

  grammar_steps->AddPage(testWindow, "Produktionen hinzufügen", true);
  grammar_steps->AddPage(new wxNotebookPage(this->grammar_steps, wxID_ANY),
                         "Übersicht", true);

  this->grammar_steps->Layout();

  this->sizer->Add(this->grammar_steps);

  // this->grammar_steps->AddChild(alpha_manager);

  // this->sizer->Add(grammar_steps);

  /*  wxBoxSizer* rule_entry_sizer = new wxBoxSizer(wxHORIZONTAL);

    this->lhs_selector =
        new wxComboBox(this, wxID_ANY, "LHS", wxDefaultPosition, wxDefaultSize);
    rule_entry_sizer->Add(
        new wxStaticText(this, wxID_ANY, "Produktionsregel eingeben:"));
    rule_entry_sizer->Add(this->lhs_selector);

    sizer->Add(alphabet_entry_sizer);
    sizer->Add(alphabet_display_sizer);*/
  // sizer->Add(rule_entry_sizer);
  SetSizer(this->sizer);
}

void AlphabetManager::add_symbol(wxCommandEvent& evt)
{
  Refresh();
}

void AlphabetManager::on_refresh(wxPaintEvent& evt)
{
  std::cout << "On refresh.\n";
}