#ifndef MANAGER_TAB_HPP
#define MANAGER_TAB_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class ManagerTab : public wxPanel
{
public:
  ManagerTab();

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  void on_page_changed(wxChildFocusEvent& evt);

  wxDECLARE_DYNAMIC_CLASS(ManagerTab);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef MANAGER_TAB_HPP */

