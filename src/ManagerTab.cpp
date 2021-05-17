#include "ManagerTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ManagerTab);

wxIMPLEMENT_DYNAMIC_CLASS(ManagerTab, wxPanel);

BEGIN_EVENT_TABLE(ManagerTab, wxPanel)
EVT_WINDOW_CREATE(ManagerTab::on_create)
EVT_CHILD_FOCUS(ManagerTab::on_page_changed)
// EVT_BUTTON(ManagerTab::add_symbol)

EVT_PAINT(ManagerTab::on_refresh)

END_EVENT_TABLE()

ManagerTab::ManagerTab()
   // : grammar_steps(new wxNotebook(this, wxID_ANY, wxDefaultPosition,
     //                              wxSize(this->GetSize())))//,
     // alpha_manager(new AlphabetManager(this->grammar_steps, wxID_ANY))
{
  Show();
}

void ManagerTab::on_page_changed(wxChildFocusEvent& evt)
{
  Layout();
}

void ManagerTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  this->grammar_steps = new wxNotebook(this, wxID_ANY, wxDefaultPosition,
                                       wxSize(this->GetSize()));
  //grammar_steps->AddPage(this->alpha_manager, "Alphabet Manager", true);

  this->sizer = new wxBoxSizer{wxVERTICAL};

  this->alpha_manager = new AlphabetManager(this->grammar_steps, wxID_ANY);
  
  wxPanel* testWindow = new wxPanel(this->grammar_steps, wxID_ANY);

  testWindow->SetBackgroundColour(wxColor(255, 0, 0));

  grammar_steps->AddPage(this->alpha_manager, "Alphabet", true);

  grammar_steps->AddPage(testWindow,
                         "Produktionen hinzufügen", false);
  grammar_steps->AddPage(new wxNotebookPage(this->grammar_steps, wxID_ANY),
                         "Übersicht", false);


  this->grammar_steps->Layout();

  this->sizer->Add(this->grammar_steps);

  //this->grammar_steps->AddChild(alpha_manager);

  //this->sizer->Add(grammar_steps);

/*  wxBoxSizer* rule_entry_sizer = new wxBoxSizer(wxHORIZONTAL);

  this->lhs_selector =
      new wxComboBox(this, wxID_ANY, "LHS", wxDefaultPosition, wxDefaultSize);
  rule_entry_sizer->Add(
      new wxStaticText(this, wxID_ANY, "Produktionsregel eingeben:"));
  rule_entry_sizer->Add(this->lhs_selector);

  sizer->Add(alphabet_entry_sizer);
  sizer->Add(alphabet_display_sizer);*/
  //sizer->Add(rule_entry_sizer);


  SetSizer(this->sizer);
}

void ManagerTab::add_symbol(wxCommandEvent& evt)
{
  bool isNonterminal;

  if (this->symbol_type_selector->GetValue() == "Nonterminal")
  {
    isNonterminal = true;
  }
  else if (this->symbol_type_selector->GetValue() == "Terminal")
  {
    isNonterminal = false;
    if (this->start_symbol_selector->GetValue())
    {
      wxMessageBox(wxT("Ein Terminal kann kein Startsymbol sein!"));
      return;
    }
  }
  else
  {
    wxMessageBox(wxT("Bitte wähle eine Symbolart aus!\n Eingabe '" +
                     this->symbol_type_selector->GetValue() +
                     "' ist nicht gültig."));
    return;
  }

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    if (this->nonterminal_alphabet.at(i).getIdentifier() == this->symbol_value_entry->GetValue())
    {
      wxMessageBox(wxT("Es gibt bereits ein Symbol mit dem Wert '" +
                       this->symbol_value_entry->GetValue() +
                       "'"));
      return;
    }
  }
  for (unsigned int i = 0; i < this->terminal_alphabet.size(); i++)
  {
    if (this->terminal_alphabet.at(i).getIdentifier() ==
        this->symbol_value_entry->GetValue())
    {
      wxMessageBox(wxT("Es gibt bereits ein Symbol mit dem Wert '" +
                       this->symbol_value_entry->GetValue() + "'"));
      return;
    }
  }

  if (isNonterminal)
  {
    this->nonterminal_alphabet.push_back(
        Nonterminal(this->symbol_value_entry->GetValue().ToStdString(), this->start_symbol_selector->GetValue()));
  }
  else
  {
    this->terminal_alphabet.push_back(
        Terminal(this->symbol_value_entry->GetValue().ToStdString(),
                 this->symbol_value_entry->GetValue().ToStdString()));
  }

  this->symbol_value_entry->SetValue("");
  this->symbol_type_selector->SetValue("");

  Refresh();
}

void ManagerTab::on_refresh(wxPaintEvent& evt)
{
  this->grammar_steps->SetSize(this->GetSize());
 /* this->lhs_selector->Clear();
  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    lhs_selector->AppendString(this->nonterminal_alphabet.at(i).getIdentifier());
  }*/
}