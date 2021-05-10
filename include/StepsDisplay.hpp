#ifndef STEPS_DISPLAY_HPP
#define STEPS_DISPLAY_HPP

#include <functional>
#include <string>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class StepsDisplay : public wxPanel
{
  public:

    using Callback = std::function<void()>;

    struct Step
    {
      bool highlight;
      std::string text;
      Callback on_click;
    };

public:
  StepsDisplay();

  void show_steps(const std::vector<Step>& steps);

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  wxDECLARE_DYNAMIC_CLASS(StepsDisplay);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef STEPS_DISPLAY_HPP */
