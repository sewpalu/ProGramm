#include "ConfigManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ConfigManager);

wxIMPLEMENT_DYNAMIC_CLASS(ConfigManager, wxFrame);

BEGIN_EVENT_TABLE(ConfigManager, wxFrame)
EVT_PAINT(ConfigManager::on_refresh)
END_EVENT_TABLE()

ConfigManager::ConfigManager() : wxFrame(NULL, wxID_ANY, "Settings", wxDefaultPosition, wxDefaultSize)
{
  this->table =
      new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(this->table, wxEXPAND);
  this->SetSizer(sizer);
  this->Show();
}


ConfigManager::~ConfigManager()
{
}

void ConfigManager::save_values(wxCommandEvent& evt)
{

}

void ConfigManager::on_refresh(wxPaintEvent& evt)
{
  nlohmann::json data = this->loader.get_content();

  this->table->ClearColumns();

  this->table->AppendTextColumn("Eigenschaft");
  this->table->AppendTextColumn("Wert");

  wxVector<wxVariant> properties;
  wxVector<wxVariant> values;

  for (auto& element : data.items())
  {
    properties.push_back(element.key());
    std::string value = element.value();
    values.push_back(value);
  }

  this->table->AppendItem(properties);
  this->table->AppendItem(values);

  this->Layout();
}