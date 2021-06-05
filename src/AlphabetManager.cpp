#include "AlphabetManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <cstring>
#include <iostream>

FORCE_LINK_ME(AlphabetManager);

wxIMPLEMENT_DYNAMIC_CLASS(AlphabetManager, wxScrolledWindow);

BEGIN_EVENT_TABLE(AlphabetManager, wxScrolledWindow)
EVT_WINDOW_CREATE(AlphabetManager::on_create)

EVT_PAINT(AlphabetManager::on_refresh)

END_EVENT_TABLE()

AlphabetManager::AlphabetManager()
{
  Show();
}

void AlphabetManager::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  SetWindowStyleFlag(wxVSCROLL);

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "manager_alphabet_panel");
  sizer->Add(panel, wxSizerFlags{}.Expand().Border(wxALL, 5).Proportion(1));
  SetSizer(sizer);

  this->symbol_type_selector = dynamic_cast<wxComboBox*>(
      FindWindowByName("alphabet_symbol_type_selector"));
  if (!this->symbol_type_selector)
  {
    std::cerr
        << "Unable to load symbol type selector in Manager - Alphabet tab.\n";
    return;
  }

  this->symbol_value_entry = dynamic_cast<wxTextCtrl*>(
      FindWindowByName("alphabet_symbol_value_entry"));
  if (!this->symbol_value_entry)
  {
    std::cerr
        << "Unable to load symbol value entry in Manager - Alphabet tab.\n";
    return;
  }

  this->start_symbol_selector = dynamic_cast<wxCheckBox*>(
      FindWindowByName("alphabet_start_symbol_selector"));
  if (!this->start_symbol_selector)
  {
    std::cerr
        << "Unable to load start symbol selector in Manager - Alphabet tab.\n";
    return;
  }

  auto* symbol_entry_button =
      dynamic_cast<wxButton*>(FindWindowByName("alphabet_symbol_entry_button"));
  if (!symbol_entry_button)
  {
    std::cerr
        << "Unable to load symbol entry button in Manager - Alphabet tab.\n";
    return;
  }
  symbol_entry_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &AlphabetManager::add_symbol, this);

  this->nonterminal_display = dynamic_cast<wxCheckListBox*>(
      FindWindowByName("alphabet_nonterminal_display"));
  if (!this->nonterminal_display)
  {
    std::cerr
        << "Unable to load nonterminal display in Manager - Alphabet tab.\n";
    return;
  }

  this->terminal_display = dynamic_cast<wxCheckListBox*>(
      FindWindowByName("alphabet_terminal_display"));
  if (!this->terminal_display)
  {
    std::cerr << "Unable to load terminal display in Manager - Alphabet tab.\n";
    return;
  }

  auto* symbol_deletion_button = dynamic_cast<wxButton*>(
      FindWindowByName("alphabet_symbol_deletion_button"));
  if (!symbol_deletion_button)
  {
    std::cerr
        << "Unable to load symbol deletion button in Manager - Alphabet tab.\n";
    return;
  }
  symbol_deletion_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                                 &AlphabetManager::delete_symbol, this);

  this->FitInside();
  this->SetScrollRate(1, 1);
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

  // Reset the value entries
  this->symbol_value_entry->SetValue("");
  this->symbol_type_selector->SetValue("");
  this->start_symbol_selector->SetValue(false);

  Refresh();
}

void AlphabetManager::delete_symbol(wxCommandEvent& evt)
{
  bool deleted_a_symbol = false;

  // Check if a Terminal is selected for deletion
  for (unsigned int i = 0; i < this->terminal_display->GetCount(); i++)
  {
    if (this->terminal_display->IsChecked(i))
    {
      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        if (std::strcmp(this->terminal_alphabet.at(j)->getIdentifier().c_str(),
                        this->terminal_display->GetString(i).c_str()) == 0)
        {
          this->terminal_alphabet.erase(this->terminal_alphabet.begin() + j);
          deleted_a_symbol = true;
        }
      }
    }
  }

  // Check if a Nonterminal is selected for deletion
  for (unsigned int i = 0; i < this->nonterminal_display->GetCount(); i++)
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

  if (!deleted_a_symbol)
    wxMessageBox(wxT("Wählen Sie ein Symbol aus, um es zu löschen!"));
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
