#ifndef STEPS_DISPLAY_HPP
#define STEPS_DISPLAY_HPP

#include <functional>
#include <string>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class StepsDisplay : wxPanel
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
  wxDECLARE_DYNAMIC_CLASS(StepsDisplay);
};

#endif /* ifndef STEPS_DISPLAY_HPP */
