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

GrammarOverviewTab::GrammarOverviewTab()
{
}

GrammarOverviewTab::GrammarOverviewTab(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL)
{

  wxWrapSizer* sizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);


  wxBoxSizer* nonterminal_sizer = new wxBoxSizer(wxVERTICAL);
  nonterminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Nichtterminale",
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_CENTER));

  this->nonterminal_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

  nonterminal_sizer->Add(nonterminal_display);

  sizer->Add(nonterminal_sizer);

  wxBoxSizer* terminal_sizer = new wxBoxSizer(wxVERTICAL);
  terminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Terminale",
                                       wxDefaultPosition, wxDefaultSize,
                                       wxALIGN_CENTER));

  this->terminal_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition,
                         wxDefaultSize);

  terminal_sizer->Add(terminal_display);

  sizer->Add(terminal_sizer);

  wxBoxSizer* production_sizer = new wxBoxSizer(wxVERTICAL);
  production_sizer->Add(new wxStaticText(this, wxID_ANY, "Produktionsregeln",
                                       wxDefaultPosition, wxDefaultSize,
                                       wxALIGN_CENTER));

  this->production_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

  production_sizer->Add(production_display);

  sizer->Add(production_sizer);

  wxBoxSizer* button_sizer = new wxBoxSizer(wxVERTICAL);

  wxButton* check_grammar_button =
      new wxButton(this, wxID_ANY, "Grammatik auf Gültigkeit überprüfen!");

  //check_grammar_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
  //                               &GrammarOverviewTab::delete_symbol, this);

  button_sizer->Add(check_grammar_button);

  sizer->Add(button_sizer);

  SetSizer(sizer);
  this->FitInside();
  this->SetScrollRate(1, 1);
}

void GrammarOverviewTab::on_create(wxWindowCreateEvent& evt)
{
}

void GrammarOverviewTab::on_refresh(wxPaintEvent& evt)
{
  /*if (this->terminal_display->GetCount() != this->terminal_alphabet.size())
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
  }*/
}