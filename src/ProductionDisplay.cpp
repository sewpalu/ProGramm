#include "ProductionDisplay.hpp"

#include "wx/html/forcelnk.h"
#include "wx/sizer.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ProductionDisplay);

// wxIMPLEMENT_DYNAMIC_CLASS(ProductionDisplay, wxPanel);

BEGIN_EVENT_TABLE(ProductionDisplay, wxPanel)
EVT_WINDOW_CREATE(ProductionDisplay::on_create)
EVT_PAINT(ProductionDisplay::on_refresh)

END_EVENT_TABLE()

ProductionDisplay::ProductionDisplay(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
  this->sizer = new wxBoxSizer{wxHORIZONTAL};

  wxBoxSizer* production_sizer = new wxBoxSizer{wxVERTICAL};
  this->m_production_display =
      new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
  production_sizer->Add(new wxStaticText(this, wxID_ANY, "Produktionen",
                                       wxDefaultPosition, wxDefaultSize));
  production_sizer->Add(this->m_production_display);
  this->sizer->Add(production_sizer, wxSizerFlags{}.Expand().Border(wxALL, 5));

  SetSizer(this->sizer);
  this->sizer->Layout();

  Show();
}

void ProductionDisplay::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;
}

void ProductionDisplay::on_refresh(wxPaintEvent& evt)
{
  this->m_production_display->Clear();
  std::vector<wxString> wx_productions;
  for (Production production : this->m_productions)
  {
    std::string production_string = "'" + production.lhs().getIdentifier() + "' -> ";

    for (size_t i = 0; i < production.rhs().size(); i++)
    {
      production_string += "'" + production.rhs().at(i)->getIdentifier() + "'";
    }
    wx_productions.push_back(wxString(production_string));
  }
  this->m_production_display->Append(wx_productions);


  // To readjust wxWrapSizer(s)
  this->SetVirtualSize(this->GetParent()->GetSize());

  this->Layout();
}


  void ProductionDisplay::set_productions(std::vector<Production> productions)
{
  this->m_productions = productions;
  this->Refresh();
}
