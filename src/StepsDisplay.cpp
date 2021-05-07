#include "StepsDisplay.hpp"

#include "wx/html/forcelnk.h"
#include "wx/statline.h"

FORCE_LINK_ME(StepsDisplay);

wxIMPLEMENT_DYNAMIC_CLASS(StepsDisplay, wxPanel);

BEGIN_EVENT_TABLE(StepsDisplay, wxPanel)
EVT_WINDOW_CREATE(StepsDisplay::on_create)
END_EVENT_TABLE()

StepsDisplay::StepsDisplay()
{
  Show();
}

void StepsDisplay::on_create(wxWindowCreateEvent&)
{
  // Flag to prevent recursively calling the create event handler when creating
  // new children.
  static auto once_flag = true;

  if (once_flag)
  {
    once_flag = false;

    auto* sizer = new wxBoxSizer{wxVERTICAL};
    auto* line = new wxStaticLine{this};
    sizer->Add(line, 0, wxEXPAND);
    SetSizer(sizer);

    once_flag = true;
  }
}

void StepsDisplay::show_steps(const std::vector<Step>& steps)
{
  // TODO
}
