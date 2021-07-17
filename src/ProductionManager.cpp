#include "ProductionManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ProductionManager);

wxIMPLEMENT_DYNAMIC_CLASS(ProductionManager, wxScrolledWindow);

BEGIN_EVENT_TABLE(ProductionManager, wxScrolledWindow)
EVT_WINDOW_CREATE(ProductionManager::on_create)
END_EVENT_TABLE()

ProductionManager::ProductionManager()
{
  Show();
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
        this->nonterminal_alphabet.at(i)->identifier);
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

  this->number_of_rhs_symbols_selector->SetRange(this->number_of_rhs_symbols_selector->GetMin(), loader.load_int_parameter("max_rhs"));

  this->rhs_container =
      dynamic_cast<wxWindow*>(FindWindowByName("production_rhs_container"));
  this->rhs_sizer = rhs_container->GetSizer();
  if (!rhs_container || !rhs_sizer)
  {
    std::cerr << "Unable to load rhs selector container in Manager - "
                 "Production tab.\n";
    return;
  }
  this->rhs_sizer->Clear(true);

  this->rhs_selectors.push_back(new wxComboBox(rhs_container, wxID_ANY));

  for (unsigned int i = 0; i < this->terminal_alphabet.size(); i++)
  {
    this->rhs_selectors.at(0)->Append(
        this->terminal_alphabet.at(i)->identifier);
  }

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    this->rhs_selectors.at(0)->Append(
        this->nonterminal_alphabet.at(i)->identifier);
  }

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

  this->update_controls();

  this->FitInside();
  this->SetScrollRate(1, 1);
}

void ProductionManager::update_controls()
{
  if (this->production_display->GetCount() != this->productions.size())
  {
    this->production_display->Clear();
    for (unsigned int i = 0; i < this->productions.size(); i++)
    {
      std::string production_string =
          "'" + this->productions.at(i).lhs().identifier + "'";
      production_string += " → ";
      for (unsigned int j = 0; j < this->productions.at(i).rhs().size(); j++)
      {
        production_string +=
            "'" + this->productions.at(i).rhs().at(j)->identifier + "' ";
      }
      this->production_display->Append(wxString::FromUTF8(production_string));
    }
  }

  this->update_symbol_selectors();

  Layout();
  Refresh();
}

void ProductionManager::add_production(wxCommandEvent& evt)
{
  if (this->lhs_selector->GetValue().ToStdString() == "")
  {

    wxMessageBox(wxString::FromUTF8(
        _("Please select a symbol for the left side of the production!")));
    return;
  }

  bool lhs_exists = false;

  for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
  {
    if (this->lhs_selector->GetValue().ToStdString() ==
        this->nonterminal_alphabet.at(i)->identifier)
      lhs_exists = true;
  }

  if (!lhs_exists)
  {
    wxString msg_output;
    msg_output.Printf(
        wxString::FromUTF8(_("The symbol '%s' is not in the alphabet. Please "
                             "add it to use it.")),
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
      wxMessageBox(wxString::FromUTF8(_(
          "A symbol has to be selected for each position. Alternatively the "
          "amount of symbols to the right of the production can be reduced.")));
      return;
    }
    bool exists_in_alphabet = false;
    for (unsigned int j = 0; j < this->nonterminal_alphabet.size(); j++)
    {
      if (this->rhs_selectors.at(i)->GetValue().ToStdString() ==
          this->nonterminal_alphabet.at(j)->identifier)
      {
        rhs.push_back(
            new Nonterminal(this->nonterminal_alphabet.at(j)->identifier));
        exists_in_alphabet = true;
      }
    }
    for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
    {
      if (this->rhs_selectors.at(i)->GetValue().ToStdString() ==
          this->terminal_alphabet.at(j)->identifier)
      {
        rhs.push_back(
            new Terminal(this->terminal_alphabet.at(j)->identifier,
                         this->terminal_alphabet.at(j)->identifier));
        exists_in_alphabet = true;
      }
    }
    if (!exists_in_alphabet)
    {
      wxString msg_output;
      msg_output.Printf(
          wxString::FromUTF8(_("The symbol '%s' is not in the alphabet. Please "
                               "add it to use it.")),
          this->rhs_selectors.at(i)->GetValue().ToStdString());
      wxMessageBox(msg_output);
      return;
    }
  }

  productions.push_back(Production(lhs, rhs));

  update_controls();

  ConfigManager* manager = new ConfigManager();
}

void ProductionManager::set_terminal_alphabet(
    std::vector<Terminal*> terminal_alphabet)
{
  this->terminal_alphabet = terminal_alphabet;
  update_controls();
}

void ProductionManager::set_nonterminal_alphabet(
    std::vector<Nonterminal*> nonterminal_alphabet)
{
  this->nonterminal_alphabet = nonterminal_alphabet;
  update_controls();
}

void ProductionManager::draw_arrow(wxBufferedPaintDC* dc)
{
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
  dc->SetPen(wxPen(wxColor(255, 0, 0), 5, wxPENSTYLE_SOLID));
  dc->DrawSpline(7, arrow_edges);
}

void ProductionManager::update_symbol_selectors()
{
  if (this->lhs_selector->GetCount() != this->nonterminal_alphabet.size())
  {
    this->lhs_selector->Clear();
    for (unsigned int i = 0; i < this->nonterminal_alphabet.size(); i++)
    {
      this->lhs_selector->Append(
          this->nonterminal_alphabet.at(i)->identifier);
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
            this->nonterminal_alphabet.at(j)->identifier);
      }
      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        this->rhs_selectors.at(i)->Append(
            this->terminal_alphabet.at(j)->identifier);
      }
    }
  }

  if (this->rhs_selectors.size() <
      (int)this->number_of_rhs_symbols_selector->GetValue())
  {
    for (unsigned int i = rhs_selectors.size();
         i < this->number_of_rhs_symbols_selector->GetValue(); i++)
    {
      this->rhs_selectors.push_back(new wxComboBox(rhs_container, wxID_ANY));

      for (unsigned int j = 0; j < this->terminal_alphabet.size(); j++)
      {
        this->rhs_selectors.back()->Append(
            this->terminal_alphabet.at(j)->identifier);
      }

      for (unsigned int j = 0; j < this->nonterminal_alphabet.size(); j++)
      {
        this->rhs_selectors.back()->Append(
            this->nonterminal_alphabet.at(j)->identifier);
      }
    }
  }
  else if (this->rhs_selectors.size() >
           (int)this->number_of_rhs_symbols_selector->GetValue())
  {
    while (this->rhs_selectors.size() > 1 &&
           this->rhs_selectors.size() >
               (int)this->number_of_rhs_symbols_selector->GetValue())
    {
      delete this->rhs_selectors.back();
      this->rhs_selectors.pop_back();
    }
  }

  this->rhs_sizer->Clear();
  for (unsigned int i = 0; i < this->rhs_selectors.size(); i++)
  {
    this->rhs_sizer->Add(this->rhs_selectors.at(i),
                         wxSizerFlags{}.Border(wxALL, 5).CentreVertical());
  }
  rhs_container->Layout();
  // rhs_sizer->Fit(rhs_container);
  rhs_container->Fit();
  rhs_container->FitInside();
  // rhs_container->SetMinSize(rhs_container->GetSize());

  Layout();
  Refresh();
}

void ProductionManager::on_rhs_change(wxSpinEvent& evt)
{
  this->number_of_rhs_symbols_selector->SetRange(this->number_of_rhs_symbols_selector->GetMin(), loader.load_int_parameter("max_rhs"));
  this->update_symbol_selectors();
}

void ProductionManager::delete_production(wxCommandEvent& evt)
{
  for (int i = this->production_display->GetCount() - 1; i >= 0; i--)
  {
    if (this->production_display->IsChecked(i))
      this->productions.erase(this->productions.begin() + i);
  }

  update_controls();
}

std::vector<Production> ProductionManager::get_productions()
{
  return this->productions;
}

void ProductionManager::set_productions(std::vector<Production> productions)
{
  this->productions = productions;
  update_controls();
}
