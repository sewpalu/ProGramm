#include "GrammarOverviewTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(GrammarOverviewTab);

wxIMPLEMENT_DYNAMIC_CLASS(GrammarOverviewTab, wxScrolledWindow);

BEGIN_EVENT_TABLE(GrammarOverviewTab, wxScrolledWindow)
EVT_WINDOW_CREATE(GrammarOverviewTab::on_create)
END_EVENT_TABLE()

GrammarOverviewTab::GrammarOverviewTab()
{
  Show();
}

void GrammarOverviewTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  SetWindowStyleFlag(wxVSCROLL);

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "manager_overview_panel");
  sizer->Add(panel, wxSizerFlags{}.Expand().Border(wxALL, 5).Proportion(1));
  SetSizer(sizer);

  this->m_alpha_display = dynamic_cast<AlphabetDisplay*>(
      FindWindowByName("overview_alphabet_display"));
  if (!m_alpha_display)
  {
    std::cerr << "Unable to load alphabet display in Manager - Overview tab.\n";
    return;
  }

  this->m_prod_display = dynamic_cast<ProductionDisplay*>(
      FindWindowByName("overview_production_display"));
  if (!m_prod_display)
  {
    std::cerr
        << "Unable to load productions display in Manager - Overview tab.\n";
    return;
  }

  wxButton* check_grammar_button =
      dynamic_cast<wxButton*>(FindWindowByName("overview_check_button"));
  if (!check_grammar_button)
  {
    std::cerr << "Unable to load check button in Manager - Overview tab.\n";
    return;
  }
  check_grammar_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                               &GrammarOverviewTab::check_grammar, this);

  this->grammar_name_entry =
      dynamic_cast<wxTextCtrl*>(FindWindowByName("overview_name_entry"));
  if (!grammar_name_entry)
  {
    std::cerr << "Unable to load name entry in Manager - Overview tab.\n";
    return;
  }

  wxButton* save_grammar_button =
      dynamic_cast<wxButton*>(FindWindowByName("overview_save_button"));
  if (!save_grammar_button)
  {
    std::cerr << "Unable to load save button in Manager - Overview tab.\n";
    return;
  }
  save_grammar_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &GrammarOverviewTab::save_grammar, this);

  this->grammars_display = dynamic_cast<wxCheckListBox*>(
      FindWindowByName("overview_grammar_selection"));
  if (!grammars_display)
  {
    std::cerr
        << "Unable to load grammar selection in Manager - Overview tab.\n";
    return;
  }
  this->grammars_display->Append(this->converter.get_grammar_names());

  wxButton* load_grammar_button =
      dynamic_cast<wxButton*>(FindWindowByName("overview_load_button"));
  if (!load_grammar_button)
  {
    std::cerr << "Unable to load load button in Manager - Overview tab.\n";
    return;
  }
  load_grammar_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &GrammarOverviewTab::load_grammar, this);

  wxButton* delete_grammars_button =
      dynamic_cast<wxButton*>(FindWindowByName("overview_delete_button"));
  if (!delete_grammars_button)
  {
    std::cerr << "Unable to load save button in Manager - Overview tab.\n";
    return;
  }
  delete_grammars_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                                 &GrammarOverviewTab::delete_grammars, this);

  this->update_displays();

  this->FitInside();
  this->SetScrollRate(1, 1);
}

void GrammarOverviewTab::update_displays()
{
  this->m_prod_display->set_productions(this->productions);
  this->m_alpha_display->set_alphabet(this->nonterminal_alphabet,
                                      this->terminal_alphabet);

  if (this->grammars_display->GetCount() !=
      this->converter.get_grammar_names().size())
  {
    this->grammars_display->Clear();
    this->grammars_display->Append(this->converter.get_grammar_names());
  }

  Layout();
  Refresh();
}

void GrammarOverviewTab::set_productions(std::vector<Production> productions)
{
  this->productions = productions;
  this->update_displays();
}

void GrammarOverviewTab::set_nonterminal_alphabet(
    std::vector<Nonterminal*> nonterminals)
{
  this->nonterminal_alphabet = nonterminals;
  this->update_displays();
}

void GrammarOverviewTab::set_terminal_alphabet(std::vector<Terminal*> terminals)
{
  this->terminal_alphabet = terminals;
  this->update_displays();
}

void GrammarOverviewTab::save_grammar(wxCommandEvent& evt)
{
  std::string grammar_name;
  grammar_name = this->grammar_name_entry->GetValue();
  if (!this->grammar_name_entry->IsModified())
  {
    wxMessageBox(_("Please enter a valid grammar name!"));
    return;
  }
  else
  {
    if (this->converter.grammar_exists(grammar_name))
    {
      std::string msg_box_text =
          _("The grammar with the name '").ToStdString() + grammar_name;
      msg_box_text += _("' already exists. Do you wish to override it?");

      wxMessageDialog* overwrite_grammar_dialog = new wxMessageDialog(
          this, wxString::FromUTF8(msg_box_text), _("Confirmation"),
          wxYES_NO | wxCENTER, wxDefaultPosition);
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
  this->update_displays();
  Refresh();
}

void GrammarOverviewTab::load_grammar(wxCommandEvent& evt)
{
  std::vector<std::string> grammar_names;
  for (int i = this->grammars_display->GetCount() - 1; i >= 0; i--)
  {
    if (this->grammars_display->IsChecked(i))
    {
      grammar_names.push_back(
          this->grammars_display->GetString(i).ToStdString());
    }
  }

  if (grammar_names.size() == 0)
  {
    std::string msg_box_text = _("A grammar has to be selected to load it!").ToStdString();

    wxMessageDialog* no_grammar_selected =
        new wxMessageDialog(this, wxString::FromUTF8(msg_box_text), _("Info"),
                            wxOK | wxCENTER, wxDefaultPosition);
    no_grammar_selected->ShowModal();
    return;
  }
  else if (grammar_names.size() == 1)
  {
    GrammarConverter::GrammarStruct grammar =
        this->converter.load_grammar_from_std_file(grammar_names.at(0));
    this->productions = grammar.productions;
    this->nonterminal_alphabet = grammar.nonterminals;
    this->terminal_alphabet = grammar.terminals;
  }
  else
  {
    wxMessageDialog* too_many_grammars = new wxMessageDialog(
        this,
        wxString::FromUTF8(
            _("Exactly on grammar has to be selected to load it!")),
        _("Info"), wxOK | wxCENTER, wxDefaultPosition);
    too_many_grammars->ShowModal();
    return;
  }

  this->update_displays();
  Refresh();
  Layout();
}

void GrammarOverviewTab::delete_grammars(wxCommandEvent& evt)
{
  for (int i = this->grammars_display->GetCount() - 1; i >= 0; i--)
  {
    if (this->grammars_display->IsChecked(i))
    {
      this->converter.delete_grammar(
          this->grammars_display->GetString(i).ToStdString());
    }
  }
  this->update_displays();
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

void GrammarOverviewTab::check_grammar(wxCommandEvent& evt)
{
  std::string concluding_message =
      "Die vorliegende Grammatik hat die vorliegenden Eigenschaften:\n";

  std::vector<Symbol*> symbols;
  std::vector<std::string> duplicate_symbol_identifiers;
  for (size_t i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    symbols.push_back(this->nonterminal_alphabet.at(i));
  }
  for (size_t i = 0; i < this->terminal_alphabet.size(); i++)
  {
    symbols.push_back(this->terminal_alphabet.at(i));
  }

  for (size_t i = 0; i < symbols.size(); i++)
  {
    for (size_t j = 0; j < symbols.size(); j++)
    {
      if (i != j)
      {
        if (symbols.at(i)->identifier == symbols.at(j)->identifier)
        {
          duplicate_symbol_identifiers.push_back(
              symbols.at(i)->identifier);
        }
      }
    }
  }

  if (duplicate_symbol_identifiers.size() == 0)
  {
    concluding_message += "- Es gibt keine doppelten Symbole.\n";
  }
  else
  {
    concluding_message += "- Die folgenden Symbole kommen mehrfach vor: ";
    for (size_t i = 0; i < duplicate_symbol_identifiers.size(); i++)
    {
      concluding_message += duplicate_symbol_identifiers.at(i) + ", ";
    }
    concluding_message += "\n";
  }

  std::vector<Production> duplicate_productions;
  for (size_t i = 0; i < this->productions.size(); i++)
  {
    for (size_t j = 0; j < this->productions.size(); j++)
    {
      if (i != j)
      {
        if (this->productions.at(i).lhs().identifier ==
            this->productions.at(j).lhs().identifier)
        {
          bool rhs_equal = this->productions.at(i).rhs().size() ==
                           this->productions.at(j).rhs().size();
          for (size_t k = 0; k < this->productions.at(i).rhs().size(); k++)
          {
            if (this->productions.at(i).rhs().at(k)->identifier !=
                this->productions.at(j).rhs().at(k)->identifier)
            {
              rhs_equal = false;
              break;
            }
          }
          if (rhs_equal)
          {
            duplicate_productions.push_back(this->productions.at(i));
          }
        }
      }
    }
  }

  if (duplicate_productions.size() == 0)
  {
    concluding_message += "- Es gibt keine doppelten Produktionsregeln.\n";
  }
  else
  {
    concluding_message += "- Die folgenden Produktionen kommen mehrfach vor: ";
    for (size_t i = 0; i < duplicate_productions.size(); i++)
    {
      concluding_message += duplicate_productions.at(i).to_string() + ", ";
    }
    concluding_message += "\n";
  }

  wxMessageBox(wxString::FromUTF8(concluding_message));
}
