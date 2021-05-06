#include "StepsDisplay.hpp"

#include "wx/html/forcelnk.h"
#include <vector>

FORCE_LINK_ME(StepsDisplay);

wxIMPLEMENT_DYNAMIC_CLASS(StepsDisplay, wxPanel);

StepsDisplay::StepsDisplay()
{
  Show();
}

void StepsDisplay::show_steps(const std::vector<Step>& steps)
{
  // TODO
}
