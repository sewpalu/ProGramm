#include "ProductionManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ProductionManager);

wxIMPLEMENT_DYNAMIC_CLASS(ProductionManager, wxScrolledWindow);

BEGIN_EVENT_TABLE(ProductionManager, wxScrolledWindow)
EVT_WINDOW_CREATE(ProductionManager::on_create)

EVT_PAINT(ProductionManager::on_refresh)

END_EVENT_TABLE()

ProductionManager::ProductionManager()
{
}

void ProductionManager::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  SetWindowStyleFlag(wxVSCROLL);

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel =
      wxXmlResource::Get()->LoadPanel(this, "manager_production_panel");
  sizer->Add(panel, wxSizerFlags{}.Expand().Border(wxALL, 5).Proportion(1));
  SetSizer(sizer);

  this->lhs_selector =
      dynamic_cast<wxComboBox*>(FindWindowByName("production_lhs_selector"));
  if (!lhs_selector)
  {
    std::cerr << "Unable to load lhs selector in Manager - Production tab.\n";
    return;
  }
  this->lhs_selector->Clear();
  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    this->lhs_selector->Append(
        this->nonterminal_alphabet.at(i)->getIdentifier());
  }

  this->number_of_rhs_symbols_selector = dynamic_cast<wxSpinCtrl*>(
      FindWindowByName("production_n_symbols_selector"));
  if (!number_of_rhs_symbols_selector)
  {
    std::cerr << "Unable to load symbol count selector in Manager - Production "
                 "tab.\n";
    return;
  }
  this->number_of_rhs_symbols_selector->SetValue(1);

  this->number_of_rhs_symbols_selector->Bind<>(
      wxEVT_SPINCTRL, &ProductionManager::on_rhs_change, this);

  this->rhs_sizer = dynamic_cast<wxWrapSizer*>(
      FindWindowByName("production_rhs_sizer_dummy_item")
          ->GetContainingSizer());
  if (!rhs_sizer)
  {
    std::cerr << "Unable to load rhs selector container in Manager - "
                 "Production tab.\n";
    return;
  }

  this->rhs_selectors.push_back(
      new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize));

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

  wxButton* production_entry_button =
      dynamic_cast<wxButton*>(FindWindowByName("production_entry_button"));
  if (!production_entry_button)
  {
    std::cerr << "Unable to load entry button in Manager - Production tab.\n";
    return;
  }
  production_entry_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                                  &ProductionManager::add_production, this);

  this->production_display =
      dynamic_cast<wxCheckListBox*>(FindWindowByName("production_display"));
  if (!production_display)
  {
    std::cerr
        << "Unable to load production display in Manager - Production tab.\n";
    return;
  }

  wxButton* delete_production =
      dynamic_cast<wxButton*>(FindWindowByName("production_delete_button"));
  if (!delete_production)
  {
    std::cerr << "Unable to load delete button in Manager - Production tab.\n";
    return;
  }
  delete_production->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                            &ProductionManager::delete_production, this);

  this->FitInside();
  this->SetScrollRate(1, 1);
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
  // this->production_arrow->SetSize(this->GetSize().x*0.3,
  // this->GetSize().y*0.3); this->draw_arrow(new wxBufferedPaintDC(this));

  if (this->production_display->GetCount() != this->productions.size())
  {
    this->production_display->Clear();
    for (unsigned int i = 0; i < this->productions.size(); i++)
    {
      std::string production_string =
          "'" + this->productions.at(i).lhs().getIdentifier() + "'";
      production_string += " -> ";
      for (unsigned int j = 0; j < this->productions.at(i).rhs().size(); j++)
      {
        production_string +=
            "'" + this->productions.at(i).rhs().at(j)->getIdentifier() + "' ";
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

    wxMessageBox(wxT(
        "Wählen Sie ein Symbol für die linke Seite der Produktionsregel aus!"));
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
              "Alternativ können sie die Anzahl der Symbole auf der rechten "
              "Seite der Produktion reduzieren."));
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
            new Terminal(this->terminal_alphabet.at(j)->getIdentifier(),
                         this->terminal_alphabet.at(j)->getIdentifier()));
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

  for (unsigned int i = 0;
       i < productions.at(productions.size() - 1).rhs().size(); i++)
  {
    std::cout
        << productions.at(productions.size() - 1).rhs().at(i)->getIdentifier();
  }
  std::cout << "\n";

  Refresh();
  Layout();
}

void ProductionManager::set_terminal_alphabet(
    std::vector<Terminal*> terminal_alphabet)
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

  if (this->rhs_selectors.size() <
      (int)this->number_of_rhs_symbols_selector->GetValue())
  {
    for (unsigned int i = rhs_selectors.size();
         i < this->number_of_rhs_symbols_selector->GetValue(); i++)
    {
      this->rhs_selectors.push_back(
          new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize));

      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        this->rhs_selectors.at(this->rhs_selectors.size() - 1)
            ->Append(this->terminal_alphabet.at(j)->getIdentifier());
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
    while (this->rhs_selectors.size() > 1 &&
           this->rhs_selectors.size() >
               (int)this->number_of_rhs_symbols_selector->GetValue())
    {
      this->rhs_selectors.at(this->rhs_selectors.size() - 1)->~wxComboBox();
      this->rhs_selectors.pop_back();
      // erase(this->rhs_selectors.end());
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
  for (int i = this->production_display->GetCount() - 1; i >= 0; i--)
  {
    if (this->production_display->IsChecked(i))
      this->productions.erase(this->productions.begin() + i);
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
