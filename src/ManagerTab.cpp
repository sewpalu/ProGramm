#include "ManagerTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ManagerTab);

wxIMPLEMENT_DYNAMIC_CLASS(ManagerTab, wxPanel);

BEGIN_EVENT_TABLE(ManagerTab, wxPanel)

EVT_WINDOW_CREATE(ManagerTab::on_create)
EVT_CHILD_FOCUS(ManagerTab::on_page_changed)

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

  //Sizer to hold all contents of this tab
  this->sizer = new wxBoxSizer{wxVERTICAL};

  //The notebook which holds the individual pages to create the grammar
  this->grammar_steps = new wxNotebook(this, wxID_ANY, wxDefaultPosition,
                                       wxSize(this->GetSize()));
  this->grammar_steps->Bind<>(wxEVT_NOTEBOOK_PAGE_CHANGED,
                              &ManagerTab::page_changed, this);

  //The alphabet manager to create and manage the alphabet
  this->alpha_manager = new AlphabetManager(this->grammar_steps, wxID_ANY);
  
  grammar_steps->AddPage(this->alpha_manager, "Alphabet", true);
  this->grammar_steps->GetPage(this->grammar_steps->GetPageCount() - 1)
      ->SetLabel("alpha_manager");

  this->prod_manager = new ProductionManager(this->grammar_steps, wxID_ANY);

  grammar_steps->AddPage(prod_manager,
                         "Produktionen", false);
  grammar_steps->AddPage(new wxNotebookPage(this->grammar_steps, wxID_ANY),
                         "Übersicht", false);


  this->grammar_steps->Layout();

  this->sizer->Add(this->grammar_steps);

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


void ManagerTab::page_changed(wxBookCtrlEvent& evt)
{
  //Check that there is a valid page changed from
  if (!(evt.GetOldSelection() < 0))
  {
    //Check which page has been left, in order to save the data from that page
    if (std::strcmp(this->grammar_steps->GetPageText(evt.GetOldSelection()).c_str(),
        "Alphabet") == 0)
    {
      this->terminal_alphabet = this->alpha_manager->get_terminal_alphabet();
      this->nonterminal_alphabet =
          this->alpha_manager->get_nonterminal_alphabet();
    }
    else if (this->grammar_steps->GetPageText(evt.GetOldSelection()).c_str() ==
             "Produktionen")
    {
      //Produktionen abspeichern
      std::cout << "Von Produktionstab gewechselt";
    }
    else if (false)
    {
      //Handle final construction of a grammar
    }
  }

  if (std::strcmp(this->grammar_steps->GetPageText(evt.GetSelection()).c_str(), "Produktionen") == 0)
  {
    this->prod_manager->set_nonterminal_alphabet(this->nonterminal_alphabet);
    this->prod_manager->set_terminal_alphabet(this->terminal_alphabet);
    this->prod_manager->Refresh();
  }
}