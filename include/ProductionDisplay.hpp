#ifndef PRODUCTION_DISPLAY_HPP
#define PRODUCTION_DISPLAY_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/wrapsizer.h>

#include "Nonterminal.hpp"
#include "Terminal.hpp"
#include "Production.hpp"
#include <vector>

class ProductionDisplay : public wxPanel
{
public:
  ProductionDisplay(wxWindow* parent);

  void set_productions(std::vector<Production> productions);

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);
  void on_refresh(wxPaintEvent& evt);

  std::vector<Production> m_productions;

  wxListBox* m_production_display;

  wxBoxSizer* sizer;

  // wxDECLARE_DYNAMIC_CLASS(ProductionDisplay);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef PRODUCTION_DISPLAY_HPP */
