#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <Nonterminal.hpp>
#include <Terminal.hpp>
#include <vector>

#include "ConfigLoader.hpp"
#include "nlohmann/json.hpp"

#include <wx/dataview.h>

class ConfigManager : public wxFrame
{
public:
  ConfigManager();
  ~ConfigManager();

private:
  void save_values(wxCommandEvent& evt);

  void on_refresh(wxPaintEvent& evt);

  ConfigLoader loader = ConfigLoader();

  wxDataViewListCtrl* table;

  const wxString property_title = wxString("Eigenschaft");
  const wxString value_title = wxString("Wert");

  wxDECLARE_DYNAMIC_CLASS(ConfigManager);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef CONFIG_MANAGER_HPP */
