#include "ProductionManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ProductionManager);

wxIMPLEMENT_DYNAMIC_CLASS(ProductionManager, wxScrolledWindow);

BEGIN_EVENT_TABLE(ProductionManager, wxScrolledWindow)
EVT_WINDOW_CREATE(ProductionManager::on_create)
// EVT_BUTTON(ProductionManager::add_symbol)

EVT_PAINT(ProductionManager::on_refresh)

END_EVENT_TABLE()

ProductionManager::ProductionManager()
{
}

ProductionManager::ProductionManager(wxWindow* parent, wxWindowID id)
    : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL)
{
  // this->sizer = new wxScrolled(this, wxID_ANY, wxDefaultPosition,
  // wxDefaultSize, wxVSCROLL);

  this->sizer = new wxBoxSizer{wxVERTICAL};

  wxWrapSizer* production_entry_sizer =
      new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  this->lhs_selector = new wxComboBox(this, wxID_ANY, "Symbol ausählen!",
                                      wxDefaultPosition, wxDefaultSize);
  
  std::cout << "Nonterminal size: " << this->nonterminal_alphabet.size()
            << "\n";
  
  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    this->lhs_selector->Append(
        this->nonterminal_alphabet.at(i).getIdentifier());
    std::cout << "Alphabet " << i << " : "
              << this->nonterminal_alphabet.at(i).getIdentifier() << "\n";
  }

  production_entry_sizer->Add(this->lhs_selector);

  //this->production_arrow = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
  production_entry_sizer->Add(new wxStaticText(this, wxID_ANY, wxString(" produziert "), wxDefaultPosition, wxDefaultSize));

  this->rhs_sizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  this->rhs_selectors.push_back(new wxComboBox(
      this, wxID_ANY, "Symbol ausählen!", wxDefaultPosition, wxDefaultSize));

  for (unsigned int i = 0; i < this->terminal_alphabet.size(); i++)
  {
    this->rhs_selectors.at(0)->Append(
        this->terminal_alphabet.at(i).getIdentifier());
  }

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    this->rhs_selectors.at(0)->Append(this->nonterminal_alphabet.at(i).getIdentifier());
  }

  this->update_symbol_selectors();

  production_entry_sizer->Add(this->rhs_sizer);

  wxButton* production_entry_button =
      new wxButton(this, wxID_ANY, "Produktion hinzufügen!");

  production_entry_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &ProductionManager::add_production, this);

  production_entry_sizer->Add(production_entry_button);

  sizer->Add(production_entry_sizer);

  SetSizer(this->sizer);
  this->FitInside();
  this->SetScrollRate(1, 1);
}

void ProductionManager::on_create(wxWindowCreateEvent& evt)
{
  SetSizer(this->sizer);
}

void ProductionManager::on_refresh(wxPaintEvent& evt)
{ 
  if (this->lhs_selector->GetCount() != this->nonterminal_alphabet.size())
  {
    this->lhs_selector->Clear();
    for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
    {
      lhs_selector->Append(this->nonterminal_alphabet.at(i).getIdentifier());
    }
  }
  //this->production_arrow->SetSize(this->GetSize().x*0.3, this->GetSize().y*0.3);
  //this->draw_arrow(new wxBufferedPaintDC(this));

  this->update_symbol_selectors();
}

void ProductionManager::add_production(wxCommandEvent& evt)
{

}

void ProductionManager::set_terminal_alphabet(std::vector<Terminal> terminal_alphabet)
{
  this->terminal_alphabet = terminal_alphabet;
}

void ProductionManager::set_nonterminal_alphabet(
    std::vector<Nonterminal> nonterminal_alphabet)
{
  this->nonterminal_alphabet = nonterminal_alphabet;
}

void ProductionManager::draw_arrow(wxBufferedPaintDC* dc)
{
  std::cout << "Defining edge points\n";
  wxPoint arrow_edges[] = {
      wxPoint(0, this->production_arrow->GetSize().y * 0.8),
      wxPoint(this->production_arrow->GetSize().x * 0.8,
              this->production_arrow->GetSize().y * 0.8),
      wxPoint(this->production_arrow->GetSize().x * 0.8,
              this->production_arrow->GetSize().y),
      wxPoint(this->production_arrow->GetSize().x,
              this->production_arrow->GetSize().y * 0.5),
      wxPoint(this->production_arrow->GetSize().x * 0.8, 0),
      wxPoint(this->production_arrow->GetSize().x * 0.8,
              this->production_arrow->GetSize().y * 0.2),
      wxPoint(0, this->production_arrow->GetSize().y * 0.2)};
  std::cout << "Drawing spline\n";
  dc->SetPen(wxPen(wxColor(255, 0, 0), 5, wxPENSTYLE_SOLID));
  dc->DrawSpline(7, arrow_edges);
}

void ProductionManager::update_symbol_selectors()
{
  std::cout << "LHS: " << this->lhs_selector->GetCount() << " vs. " << this->nonterminal_alphabet.size() << "\n";
  if (this->lhs_selector->GetCount() == this->nonterminal_alphabet.size())
  {
    this->lhs_selector->Clear();
    for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
    {
      if (this->nonterminal_alphabet.at(i).isStartSymbol())
      {
        this->lhs_selector->Append(
            "Startsymbol: " + 
            this->nonterminal_alphabet.at(i).getIdentifier());
      }
      else
      {
        this->lhs_selector->Append(
            this->nonterminal_alphabet.at(i).getIdentifier());
      }
    }
  }

  if (this->rhs_selectors.at(0)->GetCount() !=
      (this->nonterminal_alphabet.size() + this->terminal_alphabet.size()))
  {
    for (unsigned int i = 0; i < this->rhs_selectors.size(); i++)
    {
      this->rhs_selectors.at(i)->Clear();
      for (unsigned int j = 0; j < this->nonterminal_alphabet.size(); j++)
      {
        if (this->nonterminal_alphabet.at(j).isStartSymbol())
        {
          this->rhs_selectors.at(i)->Append(
              "Startsymbol: " + 
              this->nonterminal_alphabet.at(j).getIdentifier());
        }
        else
        {
          this->rhs_selectors.at(i)->Append(
              this->nonterminal_alphabet.at(j).getIdentifier());
        }
      }
      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        this->rhs_selectors.at(i)->Append(
            this->terminal_alphabet.at(j).getIdentifier());
      }
    }
  }

  if (this->rhs_sizer->GetItemCount() != this->rhs_selectors.size())
  {
    this->rhs_sizer->Clear();
    for (unsigned int i = 0; i < this->rhs_selectors.size(); i++)
    {
      this->rhs_sizer->Add(this->rhs_selectors.at(i));
    }
  }
}