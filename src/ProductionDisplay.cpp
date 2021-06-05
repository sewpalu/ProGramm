#include "ProductionDisplay.hpp"

#include "wx/html/forcelnk.h"
#include "wx/sizer.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ProductionDisplay);

wxIMPLEMENT_DYNAMIC_CLASS(ProductionDisplay, wxPanel);

BEGIN_EVENT_TABLE(ProductionDisplay, wxPanel)
EVT_WINDOW_CREATE(ProductionDisplay::on_create)
EVT_PAINT(ProductionDisplay::on_refresh)

END_EVENT_TABLE()

ProductionDisplay::ProductionDisplay()
{
  Show();
}

void ProductionDisplay::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "production_display_panel");
  sizer->Add(panel, wxSizerFlags{}.Expand().Border(wxALL, 5).Proportion(1));
  SetSizer(sizer);

  this->m_production_display = dynamic_cast<wxListBox*>(
      FindWindowByName("production_productions_display"));
  if (!m_production_display)
  {
    std::cerr << "Unable to load productions display in production display.\n";
    return;
  }

  Layout();
}

void ProductionDisplay::on_refresh(wxPaintEvent& evt)
{
  this->m_production_display->Clear();
  std::vector<wxString> wx_productions;
  for (Production production : this->m_productions)
  {
    std::string production_string =
        "'" + production.lhs().getIdentifier() + "' -> ";

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
