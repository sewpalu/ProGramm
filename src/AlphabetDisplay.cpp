#include "AlphabetDisplay.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(AlphabetDisplay);

//wxIMPLEMENT_DYNAMIC_CLASS(AlphabetDisplay, wxPanel);

BEGIN_EVENT_TABLE(AlphabetDisplay, wxPanel)
EVT_WINDOW_CREATE(AlphabetDisplay::on_create)
EVT_PAINT(AlphabetDisplay::on_refresh)

END_EVENT_TABLE()

AlphabetDisplay::AlphabetDisplay(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
  this->sizer = new wxBoxSizer{wxHORIZONTAL};

  wxBoxSizer* terminal_sizer = new wxBoxSizer{wxVERTICAL};
  this->m_terminal_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
  terminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Terminale",
                                       wxDefaultPosition, wxDefaultSize),
                      0, wxEXPAND | wxLEFT | wxRIGHT, 0);
  terminal_sizer->Add(this->m_terminal_display, 0, wxEXPAND | wxALL, 0);
  this->sizer->Add(terminal_sizer, 0, wxALL, 5);

  wxBoxSizer* nonterminal_sizer = new wxBoxSizer{wxVERTICAL};
  this->m_nonterminal_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
  nonterminal_sizer->Add(new wxStaticText(this, wxID_ANY, "Nichtterminale",
                                          wxDefaultPosition, wxDefaultSize),
                         0, wxEXPAND | wxLEFT | wxRIGHT, 0);
  nonterminal_sizer->Add(this->m_nonterminal_display, 0, wxEXPAND | wxALL, 0);
  this->sizer->Add(nonterminal_sizer, 0, wxALL, 5);

  SetSizer(this->sizer);
  this->sizer->Layout();

  Show();
}

void AlphabetDisplay::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;
}

void AlphabetDisplay::on_refresh(wxPaintEvent& evt)
{
  this->m_terminal_display->Clear();
  std::vector<wxString> wx_terminals;
  for (Terminal* temp_terminal : this->m_terminals) 
  {
    wx_terminals.push_back(wxString(temp_terminal->getIdentifier()));
  }
  this->m_terminal_display->Append(wx_terminals);

  
  this->m_nonterminal_display->Clear();
  std::vector<wxString> wx_nonterminals;
  for (Nonterminal* temp_nonterminal : this->m_nonterminals)
  {
    wx_nonterminals.push_back(wxString(temp_nonterminal->getIdentifier()));
  }
  this->m_nonterminal_display->Append(wx_nonterminals);

  //To readjust wxWrapSizer(s)
  this->SetVirtualSize(this->GetParent()->GetSize());

  this->Layout();
}

void AlphabetDisplay::set_alphabet(std::vector<Nonterminal*> nonterminals,
                  std::vector<Terminal*> terminals)
{
  this->m_terminals = terminals;
  this->m_nonterminals = nonterminals;
  this->Refresh();
}