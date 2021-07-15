#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "wx/string.h"
#include "wx/stdpaths.h"
#include <wx/platinfo.h>

class ConfigLoader
{
public:
  ConfigLoader();
  ~ConfigLoader() = default;

  int load_int_parameter(std::string identifier);
  void set_int_parameter(std::string identifier, int value);

  std::string load_string_parameter(std::string identifier);
  void set_string_parameter(std::string identifier, std::string value);

  void write_json_config(nlohmann::json output_data);

  nlohmann::json get_content();

  nlohmann::json default_values;

  void set_defaults();

private:
  std::string config_file_name = wxStandardPaths::Get().GetDocumentsDir().ToStdString();
  //nlohmann::json standard_settings = "{ \"Sprache\": englisch, \"max_rhs\": 2 }"_json;
};
