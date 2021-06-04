#include "AlphabetManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>
#include <cstring>

FORCE_LINK_ME(AlphabetManager);

wxIMPLEMENT_DYNAMIC_CLASS(AlphabetManager, wxScrolledWindow);

BEGIN_EVENT_TABLE(AlphabetManager, wxScrolledWindow)
EVT_WINDOW_CREATE(AlphabetManager::on_create)
// EVT_BUTTON(AlphabetManager::add_symbol)

EVT_PAINT(AlphabetManager::on_refresh)

END_EVENT_TABLE()

AlphabetManager::AlphabetManager()
{
}


AlphabetManager::AlphabetManager(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL)
{
  //this->sizer = new wxScrolled(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);

  this->sizer = new wxBoxSizer{wxVERTICAL};

  wxWrapSizer* alphabet_entry_sizer =
      new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  alphabet_entry_sizer->Add(
      new wxStaticText(this, wxID_ANY, "Symbol hinzufügen:"));

  this->symbol_type_selector = new wxComboBox(this, wxID_ANY);
  symbol_type_selector->Append("Terminal");
  symbol_type_selector->Append("Nonterminal");

  alphabet_entry_sizer->Add(symbol_type_selector);

  
  this->symbol_value_entry = new wxTextCtrl(this, wxID_ANY);

  alphabet_entry_sizer->Add(symbol_value_entry);

  this->start_symbol_selector =
      new wxCheckBox(this, wxID_ANY, "Als Startsymbol auswählen",
                     wxDefaultPosition, wxDefaultSize);

  alphabet_entry_sizer->Add(start_symbol_selector);

  wxButton* symbol_entry_button =
      new wxButton(this, wxID_ANY, "Symbol hinzufügen!");

  symbol_entry_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &AlphabetManager::add_symbol, this);

  alphabet_entry_sizer->Add(symbol_entry_button);

  wxSizer* alphabet_display_sizer =
      new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  wxBoxSizer* nonterminal_sizer = new wxBoxSizer(wxVERTICAL);
  nonterminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Nichtterminale",
                                       wxDefaultPosition, wxDefaultSize,
                                       wxALIGN_CENTER));

  this->nonterminal_display =
      new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

  nonterminal_sizer->Add(nonterminal_display);

  alphabet_display_sizer->Add(nonterminal_sizer);

  wxBoxSizer* terminal_sizer = new wxBoxSizer(wxVERTICAL);
  terminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Terminale", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER));

  this->terminal_display = new wxCheckListBox(
      this, wxID_ANY, wxDefaultPosition,
      wxDefaultSize);  // new wxDataViewListCtrl(
      //this, wxID_ANY);  // new wxListBox(this, wxID_ANY, wxDefaultPosition,
                        // wxDefaultSize);
  terminal_sizer->Add(terminal_display);

  alphabet_display_sizer->Add(terminal_sizer);

  wxButton* symbol_deletion_button =
      new wxButton(this, wxID_ANY, "Symbole löschen!");

  symbol_deletion_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &AlphabetManager::delete_symbol, this);

  alphabet_display_sizer->Add(symbol_deletion_button);

  sizer->Add(alphabet_entry_sizer);
  sizer->Add(alphabet_display_sizer);

  SetSizer(this->sizer);
  this->FitInside();
  this->SetScrollRate(1, 1);
}

void AlphabetManager::on_create(wxWindowCreateEvent& evt)
{ 
  SetSizer(this->sizer);
}

void AlphabetManager::add_symbol(wxCommandEvent& evt)
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
    wxMessageBox(wxT("Bitte wählen Sie eine Symbolart aus!\n Eingabe '" +
                     this->symbol_type_selector->GetValue() +
                     "' ist nicht gültig."));
    return;
  }

  if (this->symbol_value_entry->GetValue() == "")
  {
    wxMessageBox(wxT("Der Wert des Symbols darf nicht leer sein: ''"));
    return;
  }

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    if (this->nonterminal_alphabet.at(i)->getIdentifier() ==
        this->symbol_value_entry->GetValue())
    {
      wxMessageBox(wxT("Es gibt bereits ein Symbol mit dem Wert '" +
                       this->symbol_value_entry->GetValue() + "'"));
      return;
    }
  }
  for (unsigned int i = 0; i < this->terminal_alphabet.size(); i++)
  {
    if (this->terminal_alphabet.at(i)->getIdentifier() ==
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
        new Nonterminal(this->symbol_value_entry->GetValue().ToStdString(),
                    this->start_symbol_selector->GetValue()));
  }
  else
  {
    this->terminal_alphabet.push_back(
        new Terminal(this->symbol_value_entry->GetValue().ToStdString(),
                 this->symbol_value_entry->GetValue().ToStdString()));
  }

  //Reset the value entries
  this->symbol_value_entry->SetValue("");
  this->symbol_type_selector->SetValue("");
  this->start_symbol_selector->SetValue(false);

  Refresh();
}

void AlphabetManager::delete_symbol(wxCommandEvent& evt)
{
  bool deleted_a_symbol = false;

  //Check if a Terminal is selected for deletion
  for (unsigned int i = 0; i < this->terminal_display->GetCount(); i++)
  {
    if (this->terminal_display->IsChecked(i))
    {
      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        if (std::strcmp(this->terminal_alphabet.at(j)->getIdentifier().c_str(), this->terminal_display->GetString(i).c_str()) == 0)
        {
          this->terminal_alphabet.erase(this->terminal_alphabet.begin() + j);
          deleted_a_symbol = true;
        }
      }
    }
  }

  //Check if a Nonterminal is selected for deletion
  for (unsigned int  i = 0; i < this->nonterminal_display->GetCount(); i++)
  {
    if (this->nonterminal_display->IsChecked(i))
    {
      for (unsigned int j = 0; j < this->nonterminal_alphabet.size(); j++)
      {
        if (std::strcmp(
                this->nonterminal_alphabet.at(j)->getIdentifier().c_str(),
                this->nonterminal_display->GetString(i).c_str()) == 0)
        {
          this->nonterminal_alphabet.erase(this->nonterminal_alphabet.begin() +
                                           j);
          deleted_a_symbol = true;
        }
      }
    }    
  }

  Refresh();

  if(!deleted_a_symbol) wxMessageBox(wxT("Wählen Sie ein Symbol aus, um es zu löschen!"));
}

void AlphabetManager::on_refresh(wxPaintEvent& evt)
{
  if (this->terminal_display->GetCount() != this->terminal_alphabet.size())
  {
    this->terminal_display->Clear();
    for (unsigned int i = 0; i < this->terminal_alphabet.size(); i++)
    {
      this->terminal_display->AppendString(
          this->terminal_alphabet.at(i)->getIdentifier());
    }
  }

  if (this->nonterminal_display->GetCount() !=
      this->nonterminal_alphabet.size())
  {
    this->nonterminal_display->Clear();
    for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
    {
      this->nonterminal_display->AppendString(
          this->nonterminal_alphabet.at(i)->getIdentifier());
    }
  }
}


std::vector<Terminal*> AlphabetManager::get_terminal_alphabet()
{
  return this->terminal_alphabet;
}

std::vector<Nonterminal*> AlphabetManager::get_nonterminal_alphabet()
{
  return this->nonterminal_alphabet;
}

Nonterminal AlphabetManager::get_start_symbol()
{
  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    if (this->nonterminal_alphabet.at(i)->isStartSymbol())
    {
      return Nonterminal(this->nonterminal_alphabet.at(i)->getIdentifier());
    }
  }

  return Nonterminal("");
}

void AlphabetManager::set_terminal_alphabet(std::vector<Terminal*> terminals)
{
  this->terminal_alphabet = terminals;
}

void AlphabetManager::set_nonterminal_alphabet(
    std::vector<Nonterminal*> nonterminals)
{
  this->nonterminal_alphabet = nonterminals;
}
