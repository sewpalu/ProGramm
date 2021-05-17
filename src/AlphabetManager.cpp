#include "AlphabetManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

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
      new wxButton(this, this->add_symbol_button_id, "Symbol hinzufügen!");

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

  sizer->Add(alphabet_entry_sizer);
  sizer->Add(alphabet_display_sizer);

  SetSizer(this->sizer);
  this->FitInside();
  this->SetScrollRate(1, 1);
}

void AlphabetManager::on_create(wxWindowCreateEvent& evt)
{
  std::cout << "On create start!\n";
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  std::cout << "On create!\n";

  this->sizer = new wxBoxSizer{wxVERTICAL};

  
  wxSizer* alphabet_entry_sizer = new wxGridSizer(3, 3, wxSize(0, 0));

  alphabet_entry_sizer->Add(
      new wxStaticText(this, wxID_ANY, "Symbol hinzufügen:"));

  this->symbol_type_selector = new wxComboBox(this, wxID_ANY);
  symbol_type_selector->Append("Terminal");
  symbol_type_selector->Append("Nonterminal");

  alphabet_entry_sizer->Add(symbol_type_selector);

  this->symbol_value_entry = new wxTextCtrl(this, wxID_ANY);

  alphabet_entry_sizer->Add(symbol_value_entry);

  alphabet_entry_sizer->AddSpacer(1);

  this->start_symbol_selector =
      new wxCheckBox(this, wxID_ANY, "Als Startsymbol auswählen",
                     wxDefaultPosition, wxDefaultSize);

  alphabet_entry_sizer->Add(start_symbol_selector);

  wxButton* symbol_entry_button =
      new wxButton(this, this->add_symbol_button_id, "Symbol hinzufügen!");

  symbol_entry_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &AlphabetManager::add_symbol, this);

  alphabet_entry_sizer->Add(symbol_entry_button);

  wxSizer* alphabet_display_sizer = new wxGridSizer(2, wxDefaultSize);

  alphabet_display_sizer->Add(nonterminal_display);

  alphabet_display_sizer->Add(terminal_display);

  wxBoxSizer* rule_entry_sizer = new wxBoxSizer(wxHORIZONTAL);

  this->lhs_selector =
      new wxComboBox(this, wxID_ANY, "LHS", wxDefaultPosition, wxDefaultSize);
  rule_entry_sizer->Add(
      new wxStaticText(this, wxID_ANY, "Produktionsregel eingeben:"));
  rule_entry_sizer->Add(this->lhs_selector);

  sizer->Add(alphabet_entry_sizer);
  sizer->Add(alphabet_display_sizer);

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
    if (this->nonterminal_alphabet.at(i).getIdentifier() ==
        this->symbol_value_entry->GetValue())
    {
      wxMessageBox(wxT("Es gibt bereits ein Symbol mit dem Wert '" +
                       this->symbol_value_entry->GetValue() + "'"));
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
        Nonterminal(this->symbol_value_entry->GetValue().ToStdString(),
                    this->start_symbol_selector->GetValue()));
  }
  else
  {
    this->terminal_alphabet.push_back(
        Terminal(this->symbol_value_entry->GetValue().ToStdString(),
                 this->symbol_value_entry->GetValue().ToStdString()));
  }

  //Reset the value entries
  this->symbol_value_entry->SetValue("");
  this->symbol_type_selector->SetValue("");
  this->start_symbol_selector->SetValue(false);

  Refresh();
}

void AlphabetManager::on_refresh(wxPaintEvent& evt)
{
  std::cout << "Refreshing\n";
  this->terminal_display->Clear();
  this->nonterminal_display->Clear();

  for (unsigned int i = 0; i < this->terminal_alphabet.size(); i++)
  {
    this->terminal_display->AppendString(
        this->terminal_alphabet.at(i).getIdentifier());
  }

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    if (this->nonterminal_alphabet.at(i).isStartSymbol())
    {
      this->nonterminal_display->AppendString(
          "Startsymbol: " + this->nonterminal_alphabet.at(i).getIdentifier());
    }
    else
    {
      this->nonterminal_display->AppendString(
          this->nonterminal_alphabet.at(i).getIdentifier());
    }
  }

  wxVector<wxVariant> terminal_data;
  terminal_data.push_back(wxVariant(false));
  terminal_data.push_back(wxVariant("row 3"));

  std::cout << "Done refreshing alpha manager\n";
}