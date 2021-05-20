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

EVT_ERASE_BACKGROUND(ManagerTab::on_erase_background)

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

  this->SetMinSize(wxSize(300,200));

  this->old_size = this->GetSize();

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

  grammar_steps->AddPage(this->prod_manager,
                         "Produktionen", false);

  this->overview_tab = new GrammarOverviewTab(this->grammar_steps, wxID_ANY);
  grammar_steps->AddPage(this->overview_tab, "Übersicht", false);


  this->grammar_steps->Layout();

  this->sizer->Add(this->grammar_steps);

  SetSizer(this->sizer);
}

void ManagerTab::on_refresh(wxPaintEvent& evt)
{
  this->grammar_steps->SetSize(this->GetSize());
}


void ManagerTab::page_changed(wxBookCtrlEvent& evt)
{
  std::cout << "Switching tab\n";
  //Check that there is a valid page changed from
  if (!(evt.GetOldSelection() < 0))
  {
    //Check which page has been left, in order to save the data from that page
    if (std::strcmp(this->grammar_steps->GetPageText(evt.GetOldSelection()).c_str(),
        "Alphabet") == 0)
    {
      std::cout << "Reading alphabet data\n";
      this->terminal_alphabet = this->alpha_manager->get_terminal_alphabet();
      this->nonterminal_alphabet =
          this->alpha_manager->get_nonterminal_alphabet();
      if (!(this->alpha_manager->get_start_symbol().getIdentifier() == ""))
      {
        this->start_symbol = this->alpha_manager->get_start_symbol();
      }
    }
    else if (this->grammar_steps->GetPageText(evt.GetOldSelection()).c_str() ==
             "Produktionen")
    {
      std::cout << "Getting productions from prod tab\n";
      this->productions = this->prod_manager->get_productions();
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

void ManagerTab::on_erase_background(wxEraseEvent& event)
{
  //Erasing the background would cause flickering of the GUI elemnts.
  //However in this case erasing is not necessary,
  //so the event is catched and not handled.
}