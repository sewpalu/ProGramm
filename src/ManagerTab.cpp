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

void ManagerTab::on_page_changed(wxChildFocusEvent&)
{
  Layout();
}

void ManagerTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "manager_panel");
  sizer->Add(panel, wxSizerFlags{}.Expand().Border(wxALL, 5).Proportion(1));
  SetSizer(sizer);
}
