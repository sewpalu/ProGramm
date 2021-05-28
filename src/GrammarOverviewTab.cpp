#include "GrammarOverviewTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(GrammarOverviewTab);

wxIMPLEMENT_DYNAMIC_CLASS(GrammarOverviewTab, wxScrolledWindow);

BEGIN_EVENT_TABLE(GrammarOverviewTab, wxScrolledWindow)
//EVT_WINDOW_CREATE(GrammarOverviewTab::on_create)
// EVT_BUTTON(GrammarOverviewTab::add_symbol)

EVT_PAINT(GrammarOverviewTab::on_refresh)

END_EVENT_TABLE()

GrammarOverviewTab::GrammarOverviewTab() : converter(this->GetParent())
{
}

GrammarOverviewTab::GrammarOverviewTab(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL),
      converter(parent)
{

  this->sizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  wxBoxSizer* nonterminal_sizer = new wxBoxSizer(wxVERTICAL);
  nonterminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Nichtterminale",
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_CENTER));

  this->nonterminal_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

  nonterminal_sizer->Add(nonterminal_display);

  this->sizer->Add(nonterminal_sizer);

  wxBoxSizer* terminal_sizer = new wxBoxSizer(wxVERTICAL);
  terminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Terminale",
                                       wxDefaultPosition, wxDefaultSize,
                                       wxALIGN_CENTER));

  this->terminal_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition,
                         wxDefaultSize);

  terminal_sizer->Add(terminal_display);

  this->sizer->Add(terminal_sizer);

  wxBoxSizer* production_sizer = new wxBoxSizer(wxVERTICAL);
  production_sizer->Add(new wxStaticText(this, wxID_ANY, "Produktionsregeln",
                                       wxDefaultPosition, wxDefaultSize,
                                       wxALIGN_CENTER));

  this->production_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

  production_sizer->Add(production_display);

  this->sizer->Add(production_sizer);

  wxBoxSizer* button_sizer = new wxBoxSizer(wxVERTICAL);

  wxButton* check_grammar_button =
      new wxButton(this, wxID_ANY, "Grammatik auf Gültigkeit überprüfen!");

  //check_grammar_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
  //                               &GrammarOverviewTab::delete_symbol, this);

  button_sizer->Add(check_grammar_button);

  this->grammar_name_entry = new wxTextCtrl(this, wxID_ANY, "Namen eingeben!",
                                      wxDefaultPosition, wxDefaultSize);

  button_sizer->Add(this->grammar_name_entry);
  
  wxButton* save_grammar_button =
      new wxButton(this, wxID_ANY, "Grammatik speichern!");

  save_grammar_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                                &GrammarOverviewTab::save_grammar, this);

  button_sizer->Add(save_grammar_button);

  this->sizer->Add(button_sizer);

  wxBoxSizer* grammar_selection_sizer = new wxBoxSizer(wxHORIZONTAL);

  this->grammars_display =
      new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, {});

  this->grammars_display->Append(this->converter.get_grammar_names());

  grammar_selection_sizer->Add(this->grammars_display);

  wxButton* load_grammar_button =
      new wxButton(this, wxID_ANY, "Diese Grammatik laden!");
  load_grammar_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &GrammarOverviewTab::load_grammar, this);

  wxButton* delete_grammars_button =
      new wxButton(this, wxID_ANY, "Diese Grammatik(en) löschen!");
  delete_grammars_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &GrammarOverviewTab::delete_grammars, this);

  wxBoxSizer* grammar_button_sizer = new wxBoxSizer(wxVERTICAL);

  grammar_button_sizer->Add(load_grammar_button);
  grammar_button_sizer->Add(delete_grammars_button);

  grammar_selection_sizer->Add(grammar_button_sizer);

  this->sizer->Add(grammar_selection_sizer);

  SetSizer(this->sizer);
  this->FitInside();
  this->SetScrollRate(1, 1);
}

void GrammarOverviewTab::on_create(wxWindowCreateEvent& evt)
{
}

void GrammarOverviewTab::on_refresh(wxPaintEvent& evt)
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
      //this->nonterminal_display->GetItem(this->nonterminal_display->position)->SetBackgroundColour(*wxRED);
      /*std::cout << "Setting the background color\n";
      this->nonterminal_display
          ->GetItem(this->nonterminal_display->GetCount() - 1)
          ->SetBackgroundColour(wxColour(255, 155, 155));
      std::cout << "Set the background color\n";*/
    }
  }

  if (this->production_display->GetCount() != this->productions.size())
  {
    this->production_display->Clear();
    for (unsigned int i = 0; i < this->productions.size(); i++)
    {
      this->production_display->Append(this->productions.at(i).to_string());
    }
  }

  if (this->grammars_display->GetCount() != this->converter.get_grammar_names().size())
  {
    this->grammars_display->Clear();
    this->grammars_display->Append(this->converter.get_grammar_names());
  }



  this->SetVirtualSize(this->GetParent()->GetSize());

  this->sizer->Layout();

  this->Layout();

  Refresh();
  Layout();
}

void GrammarOverviewTab::set_productions(std::vector<Production> productions)
{
  this->productions = productions;
}

void GrammarOverviewTab::set_nonterminal_alphabet(
    std::vector<Nonterminal*> nonterminals)
{
  this->nonterminal_alphabet = nonterminals;
}

void GrammarOverviewTab::set_terminal_alphabet(std::vector<Terminal*> terminals)
{
  this->terminal_alphabet = terminals;
}

void GrammarOverviewTab::save_grammar(wxCommandEvent& evt)
{
  std::cout << "Saving grammar\n";
  std::string grammar_name;
  grammar_name = this->grammar_name_entry->GetValue();
  if (grammar_name == "Namen eingeben!")
  {
    wxMessageBox(
        wxT("Bitte geben Sie einen gültigen Namen für die Grammnatik ein!"));
    return;
  }
  else
  {
    if (this->converter.grammar_exists(grammar_name))
    {
      std::string msg_box_text = "Die Grammatik mit dem Namen '" + grammar_name;
      msg_box_text += "' existiert bereits. Möchten Sie sie überschreiben?";
      std::cout << "Constructing parent\n";

      wxMessageDialog* overwrite_grammar_dialog =
          new wxMessageDialog(this, msg_box_text, "Caption", wxYES_NO |
                              wxCENTER, wxDefaultPosition);
      overwrite_grammar_dialog->Show();
      if (!(overwrite_grammar_dialog->ShowModal() == wxID_YES))
      {
        return;
      }
    }
    this->converter.save_grammar_to_std_file(this->nonterminal_alphabet,
                                             this->terminal_alphabet,
                                             this->productions, grammar_name);
  }
  Refresh();
}

void GrammarOverviewTab::load_grammar(wxCommandEvent& evt)
{
  std::cout << "'Loading'\n";
  std::vector<std::string> grammar_names;
  for (int i = this->grammars_display->GetCount() - 1; i >= 0; i--)
  {
    if (this->grammars_display->IsChecked(i))
    {
      grammar_names.push_back(
          this->grammars_display->GetString(i).ToStdString());
    }
  }

  std::cout << "Selected grammar names: " << grammar_names.size() << "\n";

  if (grammar_names.size() == 0)
  {
    std::string msg_box_text = "Sie müssen eine Grammatik auswählen, um sie zu laden!\n";

    wxMessageDialog* no_grammar_selected = new wxMessageDialog(
        this, msg_box_text, "Caption", wxOK | wxCENTER, wxDefaultPosition);
    no_grammar_selected->ShowModal();
    return;
  }
  else if (grammar_names.size() == 1)
  {
    GRAMMAR_STRUCT grammar =
        this->converter.load_grammar_from_std_file(grammar_names.at(0));
    this->productions = grammar.productions;
    this->nonterminal_alphabet = grammar.nonterminals;
    this->terminal_alphabet = grammar.terminals;
  }
  else
  {
    wxMessageDialog* too_many_grammars = new wxMessageDialog(
      this, "Sie müssen exakt eine Grammatik auswählen um sie zu laden!\n",
        "Caption", wxOK | wxCENTER, wxDefaultPosition);
    too_many_grammars->ShowModal();
    return;
  }

  Refresh();
  Layout();
}

void GrammarOverviewTab::delete_grammars(wxCommandEvent& evt)
{
  std::cout << "Deleting\n";
  for (int i = this->grammars_display->GetCount() - 1; i >= 0; i--)
  {
    if (this->grammars_display->IsChecked(i))
    {
      std::cout << "Checked grammar: "
                << this->grammars_display->GetString(i).ToStdString() << "\n";
      this->converter.delete_grammar(
          this->grammars_display->GetString(i).ToStdString());
    }
  }
  Refresh();
  Layout();
}

std::vector<Production> GrammarOverviewTab::get_productions()
{
  return this->productions;
}

std::vector<Nonterminal*> GrammarOverviewTab::get_nonterminal_alphabet()
{
  return this->nonterminal_alphabet;
}

std::vector<Terminal*> GrammarOverviewTab::get_terminal_alphabet()
{
  return this->terminal_alphabet;
}