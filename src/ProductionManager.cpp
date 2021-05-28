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
    : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL),
      number_of_rhs_symbols_selector(
          new wxSpinCtrl())
{
  // this->sizer = new wxScrolled(this, wxID_ANY, wxDefaultPosition,
  // wxDefaultSize, wxVSCROLL);

  this->sizer = new wxBoxSizer{wxVERTICAL};

  wxWrapSizer* production_entry_sizer =
      new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  this->lhs_selector = new wxComboBox(this, wxID_ANY, "",
                                      wxDefaultPosition, wxDefaultSize);
  
  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    this->lhs_selector->Append(
        this->nonterminal_alphabet.at(i)->getIdentifier());
  }

  production_entry_sizer->Add(this->lhs_selector);

  //this->production_arrow = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
  production_entry_sizer->Add(new wxStaticText(this, wxID_ANY, wxString(" produziert "), wxDefaultPosition, wxDefaultSize));

  this->rhs_sizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  this->rhs_selectors.push_back(new wxComboBox(
      this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize));

  for (unsigned int i = 0; i < this->terminal_alphabet.size(); i++)
  {
    this->rhs_selectors.at(0)->Append(
        this->terminal_alphabet.at(i)->getIdentifier());
  }

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    this->rhs_selectors.at(0)->Append(
        this->nonterminal_alphabet.at(i)->getIdentifier());
  }

  this->update_symbol_selectors();

  production_entry_sizer->Add(this->rhs_sizer);

  wxButton* production_entry_button =
      new wxButton(this, wxID_ANY, "Produktion hinzufügen!");

  production_entry_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                              &ProductionManager::add_production, this);

  production_entry_sizer->Add(production_entry_button);

  this->number_of_rhs_symbols_selector =
      new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                     wxSP_ARROW_KEYS, 1, 10, 1);
  this->number_of_rhs_symbols_selector->SetValue(1);

  this->number_of_rhs_symbols_selector->Bind<>(
      wxEVT_SPINCTRL,
                                  &ProductionManager::on_rhs_change, this);

  production_entry_sizer->Add(this->number_of_rhs_symbols_selector);

  sizer->Add(production_entry_sizer);
  
  wxWrapSizer* production_display_sizer =
      new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

  this->production_display = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

  production_display_sizer->Add(this->production_display);

  wxButton* delete_production = new wxButton(this, wxID_ANY, "Regel löschen",
                                             wxDefaultPosition, wxDefaultSize);

  delete_production->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                                  &ProductionManager::delete_production, this);

  production_display_sizer->Add(delete_production);

  sizer->Add(production_display_sizer);

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
    std::cout << "Clearing in refresh\n";
    this->lhs_selector->Clear();
    for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
    {
      this->lhs_selector->Append(
          this->nonterminal_alphabet.at(i)->getIdentifier());
    }
  }
  //this->production_arrow->SetSize(this->GetSize().x*0.3, this->GetSize().y*0.3);
  //this->draw_arrow(new wxBufferedPaintDC(this));

  if (this->production_display->GetCount() != this->productions.size())
  {
    this->production_display->Clear();
    for (unsigned int i = 0; i < this->productions.size(); i++)
    {
      std::string production_string =
          "'" + this->productions.at(i).lhs.getIdentifier() + "'";
      production_string += " -> ";
      for (unsigned int j = 0; j < this->productions.at(i).rhs.size(); j++)
      {
        production_string +=
            "'" + this->productions.at(i).rhs.at(j)->getIdentifier() + "' ";
      }
      this->production_display->Append(production_string);
    }
  }

  this->SetVirtualSize(this->GetParent()->GetSize());

  this->update_symbol_selectors();

  Layout();
}

void ProductionManager::add_production(wxCommandEvent& evt)
{
  std::cout << "Adding production.\n";
  if (this->lhs_selector->GetValue().ToStdString() == "")
  {

    wxMessageBox(
        wxT("Wählen Sie ein Symbol für die linke Seite der Produktionsregel aus!"));
    return;
  }

  bool lhs_exists = false;

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    if (this->lhs_selector->GetValue().ToStdString() ==
        this->nonterminal_alphabet.at(i)->getIdentifier())
      lhs_exists = true;
  }

  if (!lhs_exists)
  {
    wxString msg_output;
    msg_output.Printf(wxT("Das Symbol '%s"
                          "' ist nicht im Alphabet enthalten."
                          "Fügen Sie es hinzu, um es zu "
                          "verwenden."),
                      this->lhs_selector->GetValue().ToStdString());
    wxMessageBox(msg_output);
    return;
  }

  Nonterminal lhs(this->lhs_selector->GetValue().ToStdString());
  std::vector<Symbol*> rhs;

  for (unsigned int i = 0; i < this->rhs_selectors.size(); i++)
  {
    if (this->rhs_selectors.at(i)->GetValue().ToStdString() == "")
    {
      wxMessageBox(
        wxT("Sie müssen für jede Auswahlmöglichkeit ein Symbol auswählen. "
          "Alternativ können sie die Anzahl der Symbole auf der rechten Seite der Produktion reduzieren."));
      return;
    }
    bool exists_in_alphabet = false;
    for (unsigned int j = 0; j < this->nonterminal_alphabet.size(); j++)
    {
      std::cout << this->rhs_selectors.at(i)->GetValue().ToStdString()
                << " vs. " << this->nonterminal_alphabet.at(j)->getIdentifier();
      if (this->rhs_selectors.at(i)->GetValue().ToStdString() ==
          this->nonterminal_alphabet.at(j)->getIdentifier())
      {
        std::cout << "Pushing back\n";
        rhs.push_back(
            new Nonterminal(this->nonterminal_alphabet.at(j)->getIdentifier()));
        exists_in_alphabet = true;
      }
      std::cout << "\n";
    }
    for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
    {
      if (this->rhs_selectors.at(i)->GetValue().ToStdString() ==
          this->terminal_alphabet.at(j)->getIdentifier())
      {
        rhs.push_back(
            new Terminal(this->terminal_alphabet.at(j)->getIdentifier(), this->terminal_alphabet.at(j)->getIdentifier()));
        exists_in_alphabet = true;
      }
    }
    if (!exists_in_alphabet)
    {
      wxString msg_output;
      msg_output.Printf(wxT("Das Symbol '%s"
                            "' ist nicht im Alphabet enthalten."
                            "Fügen Sie es hinzu, um es zu "
                            "verwenden."),
                        this->rhs_selectors.at(i)->GetValue().ToStdString());
      wxMessageBox(msg_output);
      return;
    }
  }

  productions.push_back(Production(lhs, rhs));
  std::cout << "Added production: " << lhs.getIdentifier() << " -> ";

  for (unsigned int i = 0; i < productions.at(productions.size() - 1).rhs.size(); i++)
  {
    std::cout << productions.at(productions.size() - 1).rhs.at(i)->getIdentifier();
  }
  std::cout << "\n";

  Refresh();
  Layout();
}

void ProductionManager::set_terminal_alphabet(std::vector<Terminal*> terminal_alphabet)
{
  this->terminal_alphabet = terminal_alphabet;
}

void ProductionManager::set_nonterminal_alphabet(
    std::vector<Nonterminal*> nonterminal_alphabet)
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
  if (this->lhs_selector->GetCount() != this->nonterminal_alphabet.size())
  {
    std::cout << "Clearing in update symbol selectors\n";
    this->lhs_selector->Clear();
    for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
    {
      this->lhs_selector->Append(
          this->nonterminal_alphabet.at(i)->getIdentifier());
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
        this->rhs_selectors.at(i)->Append(
            this->nonterminal_alphabet.at(j)->getIdentifier());
      }
      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        this->rhs_selectors.at(i)->Append(
            this->terminal_alphabet.at(j)->getIdentifier());
      }
    }
  }

  if (this->rhs_selectors.size() < (int)this->number_of_rhs_symbols_selector->GetValue())
  {
    for (unsigned int i = rhs_selectors.size();
         i < this->number_of_rhs_symbols_selector->GetValue(); i++)
    {
      this->rhs_selectors.push_back(
          new wxComboBox(this, wxID_ANY, "", wxDefaultPosition,
                         wxDefaultSize));

      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        this->rhs_selectors.at(this->rhs_selectors.size() - 1)->Append(
            this->terminal_alphabet.at(j)->getIdentifier());
      }

      for (unsigned int j = 0; j < this->nonterminal_alphabet.size(); j++)
      {
        this->rhs_selectors.at(this->rhs_selectors.size() - 1)
            ->Append(this->nonterminal_alphabet.at(j)->getIdentifier());
      }
    }
    Layout();
  }
  else if (this->rhs_selectors.size() >
           (int)this->number_of_rhs_symbols_selector->GetValue())
  {
    while (this->rhs_selectors.size() > 1 && this->rhs_selectors.size() >
           (int)this->number_of_rhs_symbols_selector->GetValue())
    {
      this->rhs_selectors.at(this->rhs_selectors.size() - 1)->~wxComboBox();
      this->rhs_selectors.pop_back();
       //erase(this->rhs_selectors.end());
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
  this->SetVirtualSize(this->GetParent()->GetSize());

  Refresh();
  Layout();
}

void ProductionManager::on_rhs_change(wxSpinEvent& evt)
{
  this->update_symbol_selectors();
}

void ProductionManager::delete_production(wxCommandEvent& evt)
{
  std::cout << "production_display->GetCount() = "
            << this->production_display->GetCount() << "\n";
  std::cout << "productions.size() = " << this->productions.size() << "\n";
  for (int i = this->production_display->GetCount() -1;
       i >= 0; i--)
  {
    if (this->production_display->IsChecked(i)) this->productions.erase(this->productions.begin() + i);
  }
  Refresh();
  Layout();
}

std::vector<Production> ProductionManager::get_productions()
{
  return this->productions;
}

void ProductionManager::set_productions(std::vector<Production> productions)
{
  this->productions = productions;
}