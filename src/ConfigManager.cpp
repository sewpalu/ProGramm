#include "ConfigManager.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <wx/textctrl.h>

#include <iostream>
#include <string>

FORCE_LINK_ME(ConfigManager);

wxIMPLEMENT_DYNAMIC_CLASS(ConfigManager, wxScrolledWindow);

BEGIN_EVENT_TABLE(ConfigManager, wxScrolledWindow)
EVT_PAINT(ConfigManager::on_refresh)
EVT_WINDOW_CREATE(ConfigManager::on_create)
END_EVENT_TABLE()

ConfigManager::ConfigManager()
{
  this->Show();
}

void ConfigManager::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  nlohmann::json data = this->loader.get_content();

  int collumns = 2;
  int rows = data.size();

  this->table = new wxGridSizer(rows + 1, collumns, 5, 5);
  this->SetBackgroundColour(wxColour(wxString("white")));

  for (auto& element : data.items())
  {
    std::pair<wxStaticText*, std::pair<wxTextCtrl*, std::string>> temp;
    wxStaticText* label = new wxStaticText(this, wxID_ANY, element.key());
    this->table->Add(label, wxEXPAND, wxALIGN_LEFT);
    temp.first = label;
    std::string value = "";

    if (element.value().is_string())
    {
      temp.second.second = "string";
      value = element.value();
    }
    else if (element.value().is_number_integer())
    {
      temp.second.second = "int";
      int temp = element.value();
      value = std::to_string(temp);
    }
    else if (element.value().is_number_float())
    {
      temp.second.second = "float";
      float temp = element.value();
      value = std::to_string(temp);
    }
    else if (element.value().is_boolean())
    {
      temp.second.second = "bool";
      if (element.value())
      {
        value = "true";
      }
      else
      {
        value = "false";
      }
    }

    wxTextCtrl* ctrl =
        new wxTextCtrl(this, wxID_ANY, value, wxDefaultPosition, wxDefaultSize);
    this->table->Add(ctrl, wxEXPAND, wxALIGN_LEFT);
    temp.second.first = ctrl;
    this->content.push_back(temp);
  }

  this->save_button = new wxButton(this, wxID_ANY, "Speichern!",
                                       wxDefaultPosition, wxDefaultSize);
  save_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED, &ConfigManager::save, this);
  this->table->Add(save_button, wxEXPAND, wxALIGN_LEFT);

  this->reset_button =
      new wxButton(this, wxID_ANY, "Standardwerte wiederherstellen!",
                   wxDefaultPosition, wxDefaultSize);
   reset_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
   &ConfigManager::load_default,
                       this);

  this->table->Add(reset_button, wxEXPAND, wxALIGN_LEFT);


  this->table->SetSizeHints(this);

  this->SetSizer(this->table, wxALL | 10);

  this->FitInside();
  this->SetScrollRate(1, 1);
}

void ConfigManager::save(wxCommandEvent& evt)
{
  nlohmann::json old_data = this->loader.get_content();

  for (size_t i = 0; i < this->content.size(); i++)
  {
    if (this->content.at(i).second.second == "string")
    {
      old_data[this->content.at(i).first->GetLabelText().ToStdString()] =
          this->content.at(i).second.first->GetValue().ToStdString();
    }
    else if (this->content.at(i).second.second == "int")
    {
      old_data[this->content.at(i).first->GetLabelText().ToStdString()] =
          std::stoi(this->content.at(i).second.first->GetValue().ToStdString());
    }
    else if (this->content.at(i).second.second == "float")
    {
      old_data[this->content.at(i).first->GetLabelText().ToStdString()] =
          std::stof(this->content.at(i).second.first->GetValue().ToStdString());
    }
    else if (this->content.at(i).second.second == "bool")
    {
      old_data[this->content.at(i).first->GetLabelText().ToStdString()] =
          this->content.at(i).second.first->GetValue().ToStdString() ==
          "true";
    }
  }

  this->loader.write_json_config(old_data);
}

void ConfigManager::load_default(wxCommandEvent& evt)
{
  this->loader.set_defaults();

  for (size_t i = 0; i < this->content.size(); i++)
  {
    this->content.at(i).first->Destroy();
    this->content.at(i).second.first->Destroy();
  }

  this->content.clear();

  this->save_button->Destroy();
  this->reset_button->Destroy();

  this->table->Clear();

  nlohmann::json data = this->loader.get_content();

  int collumns = 2;
  int rows = data.size();

  this->SetBackgroundColour(wxColour(wxString("white")));

  for (auto& element : data.items())
  {
    std::pair<wxStaticText*, std::pair<wxTextCtrl*, std::string>> temp;
    wxStaticText* label = new wxStaticText(this, wxID_ANY, element.key());
    this->table->Add(label, wxEXPAND, wxALIGN_LEFT);
    temp.first = label;
    std::string value = "";

    if (element.value().is_string())
    {
      temp.second.second = "string";
      value = element.value();
    }
    else if (element.value().is_number_integer())
    {
      temp.second.second = "int";
      int temp = element.value();
      value = std::to_string(temp);
    }
    else if (element.value().is_number_float())
    {
      temp.second.second = "float";
      float temp = element.value();
      value = std::to_string(temp);
    }
    else if (element.value().is_boolean())
    {
      temp.second.second = "bool";
      if (element.value())
      {
        value = "true";
      }
      else
      {
        value = "false";
      }
    }

    wxTextCtrl* ctrl =
        new wxTextCtrl(this, wxID_ANY, value, wxDefaultPosition, wxDefaultSize);
    this->table->Add(ctrl, wxEXPAND, wxALIGN_LEFT);
    temp.second.first = ctrl;
    this->content.push_back(temp);
  }

  this->save_button = new wxButton(this, wxID_ANY, "Speichern!",
                                       wxDefaultPosition, wxDefaultSize);
  save_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED, &ConfigManager::save, this);
  this->table->Add(save_button, wxEXPAND, wxALIGN_LEFT);

  this->reset_button =
      new wxButton(this, wxID_ANY, "Standardwerte wiederherstellen!",
                   wxDefaultPosition, wxDefaultSize);
  reset_button->Bind<>(wxEVT_COMMAND_BUTTON_CLICKED,
                       &ConfigManager::load_default, this);

  this->table->Add(reset_button, wxEXPAND, wxALIGN_LEFT);
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::save_values(wxCommandEvent& evt)
{

}

void ConfigManager::on_refresh(wxPaintEvent& evt)
{
  this->SetVirtualSize(this->GetSize());

  this->Layout();
}