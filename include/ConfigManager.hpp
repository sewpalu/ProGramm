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

class ConfigManager : public wxScrolledWindow
{
public:
  ConfigManager();
  ~ConfigManager();

private:
  void on_create(wxWindowCreateEvent& evt);

  void save_values(wxCommandEvent& evt);

  void load_default(wxCommandEvent& evt);

  void on_refresh(wxPaintEvent& evt);

  void save(wxCommandEvent& evt);

  ConfigLoader loader = ConfigLoader();

  wxGridSizer* table;
  //Storage for the content of the Content Manager
  //For each item the name is stored as wxStaticText and the value is stored as wxTextCtrl
  //The data type is stored as string
  std::vector<std::pair<wxStaticText*, std::pair<wxTextCtrl*, std::string>>> content;

  
  wxButton* save_button;
  wxButton* reset_button;

  const wxString property_title = wxString("Eigenschaft");
  const wxString value_title = wxString("Wert");

  wxDECLARE_DYNAMIC_CLASS(ConfigManager);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef CONFIG_MANAGER_HPP */
