#include "ManagerTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(ManagerTab);

wxIMPLEMENT_DYNAMIC_CLASS(ManagerTab, wxPanel);

BEGIN_EVENT_TABLE(ManagerTab, wxPanel)
EVT_WINDOW_CREATE(ManagerTab::on_create)
EVT_CHILD_FOCUS(ManagerTab::on_page_changed)
END_EVENT_TABLE()

ManagerTab::ManagerTab()
{
  Show();
}

void ManagerTab::on_page_changed(wxChildFocusEvent& evt)
{
  Layout();
}

void ManagerTab::on_create(wxWindowCreateEvent&)
{
  // Flag to prevent recursively calling the create event handler when creating
  // new children.
  static auto once_flag = true;

  if (once_flag)
  {
    once_flag = false;
    auto* sizer = new wxBoxSizer{wxVERTICAL};
    auto* panel = wxXmlResource::Get()->LoadPanel(this, "manager_panel");
    sizer->Add(panel, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);
  }
}
