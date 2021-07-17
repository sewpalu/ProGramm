#include "AlphabetDisplay.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(AlphabetDisplay);

wxIMPLEMENT_DYNAMIC_CLASS(AlphabetDisplay, wxPanel);

BEGIN_EVENT_TABLE(AlphabetDisplay, wxPanel)
EVT_WINDOW_CREATE(AlphabetDisplay::on_create)
END_EVENT_TABLE()

AlphabetDisplay::AlphabetDisplay()
{
  Show();
}

void AlphabetDisplay::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "alphabet_display_panel");
  sizer->Add(panel, wxSizerFlags{}.Expand().Border(wxALL, 5).Proportion(1));
  SetSizer(sizer);

  this->m_terminal_display =
      dynamic_cast<wxListBox*>(FindWindowByName("alphabet_terminals_display"));
  if (!m_terminal_display)
  {
    std::cerr << "Unable to load terminal display in alphabet display.\n";
    return;
  }

  this->m_nonterminal_display = dynamic_cast<wxListBox*>(
      FindWindowByName("alphabet_nonterminals_display"));
  if (!m_nonterminal_display)
  {
    std::cerr << "Unable to load non-terminal display in alphabet display.\n";
    return;
  }

  Layout();
}

void AlphabetDisplay::set_alphabet(std::vector<Nonterminal*> nonterminals,
                                   std::vector<Terminal*> terminals)
{
  //Simply set the parameters
  this->m_terminals = terminals;
  this->m_nonterminals = nonterminals;

  //Update the display content
  this->m_terminal_display->Clear();
  std::vector<wxString> wx_terminals;
  for (Terminal* temp_terminal : this->m_terminals)
  {
    wx_terminals.push_back(wxString(temp_terminal->identifier));
  }
  this->m_terminal_display->Append(wx_terminals);

  this->m_nonterminal_display->Clear();
  std::vector<wxString> wx_nonterminals;
  for (Nonterminal* temp_nonterminal : this->m_nonterminals)
  {
    wx_nonterminals.push_back(wxString(temp_nonterminal->identifier));
  }
  this->m_nonterminal_display->Append(wx_nonterminals);

  this->Layout();
  this->Refresh();
}
