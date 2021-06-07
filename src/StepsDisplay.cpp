#include "StepsDisplay.hpp"

#include "wx/control.h"
#include "wx/html/forcelnk.h"
#include "wx/sizer.h"
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

void StepsDisplay::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxHORIZONTAL};
  auto* line = new wxStaticLine{this};
  sizer->Add(line, wxSizerFlags{}.Proportion(1));
  SetSizer(sizer);
}

void StepsDisplay::show_steps(const std::vector<Step>& steps)
{
  auto* sizer = GetSizer();
  if (!sizer)
    return;

  sizer->Clear(true);

  auto* line = new wxStaticLine{this};
  sizer->Add(line, wxSizerFlags{}.Proportion(1).Align(wxALIGN_CENTRE_VERTICAL));

  for (const auto& step : steps)
  {
    auto* indicator = static_cast<wxControl*>(nullptr);
    if (step.on_click)
    {
      auto* button = new wxButton{this, wxID_ANY};
      button->Bind(wxEVT_BUTTON,
                   [handler = step.on_click](const auto&) { handler(); });
      indicator = button;
    }
    else
    {
      auto text = new wxStaticText{this, wxID_ANY, ""};
      text->Wrap(-1);
      indicator = text;
    }

    indicator->SetLabelMarkup(wxString::FromUTF8(step.text));
    if (step.highlight)
      indicator->SetBackgroundColour(*wxGREEN);

    sizer->Add(indicator,
               wxSizerFlags{}.Border(wxALL, 5).Align(wxALIGN_CENTRE_VERTICAL));

    auto* line = new wxStaticLine{this};
    sizer->Add(line, wxSizerFlags{}.Border(wxALL, 5).Proportion(1).Align(
                         wxALIGN_CENTRE_VERTICAL));
  }

  SetSizer(sizer);
  Layout();
}
