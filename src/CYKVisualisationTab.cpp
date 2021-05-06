#include "CYKVisualisationTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

FORCE_LINK_ME(CYKVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(CYKVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(CYKVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(CYKVisualisationTab::on_create)
END_EVENT_TABLE()

CYKVisualisationTab::CYKVisualisationTab()
{
  Show();
}

void CYKVisualisationTab::update_input(const FormalGrammar& grammar,
                                       const Word& word)
{
  // TODO
}

void CYKVisualisationTab::on_create(wxWindowCreateEvent&)
{
  // Flag to prevent recursively calling the create event handler when creating
  // new children.
  static auto once_flag = true;

  if (once_flag)
  {
    once_flag = false;

    auto* sizer = new wxBoxSizer{wxVERTICAL};
    sizer->Add(wxXmlResource::Get()->LoadPanel(this, "cyk_panel"), 1, wxEXPAND);
    SetSizer(sizer);
  }
}
