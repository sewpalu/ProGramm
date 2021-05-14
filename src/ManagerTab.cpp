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

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  //auto* panel = wxXmlResource::Get()->LoadPanel(this, "manager_panel");
  //panel->SetBackgroundColour(wxColor(255, 0, 0));
//  sizer->Add(panel, 1, wxEXPAND | wxALL, 5);

  wxSizer* rule_entry_sizer = new wxGridSizer(3,4, wxSize(0,0));

  rule_entry_sizer->Add(new wxStaticText(this, wxID_ANY, "Symbol hinzufügen:"));

  this->symbol_type_selector = new wxComboBox(this, wxID_ANY);
  symbol_type_selector->Append("Terminal");
  symbol_type_selector->Append("Nonterminal");

  rule_entry_sizer->Add(symbol_type_selector);

  this->symbol_value_entry = new wxTextCtrl(this, wxID_ANY);

  rule_entry_sizer->Add(symbol_value_entry);

  wxButton* symbol_entry_button =
      new wxButton(this, this->add_symbol_button_id, "Symbol hinzufügen!");

  symbol_entry_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED, &ManagerTab::add_symbol, this);

  rule_entry_sizer->Add(symbol_entry_button);

  sizer->Add(rule_entry_sizer);
  SetSizer(sizer);
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
        Nonterminal(this->symbol_value_entry->GetValue().ToStdString()));
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
  std::cout << "Refreshing\n";
}