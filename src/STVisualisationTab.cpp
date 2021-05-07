#include "STVisualisationTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(STVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(STVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(STVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(STVisualisationTab::on_create)
EVT_CHILD_FOCUS(STVisualisationTab::on_page_changed)
END_EVENT_TABLE()

STVisualisationTab::STVisualisationTab()
{
  Show();
}

void STVisualisationTab::update_input(const FormalGrammar& grammar,
                                      const Word& word)
{
  // TODO
}

void STVisualisationTab::on_page_changed(wxChildFocusEvent&)
{
  Layout();
}

void STVisualisationTab::on_create(wxWindowCreateEvent&)
{
  // Flag to prevent recursively calling the create event handler when creating
  // new children.
  static auto once_flag = true;

  if (once_flag)
  {
    once_flag = false;
    auto* sizer = new wxBoxSizer{wxVERTICAL};
    auto* panel = wxXmlResource::Get()->LoadPanel(this, "st_panel");
    sizer->Add(panel, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);
  }
}
