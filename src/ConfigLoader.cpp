#include "ConfigLoader.hpp"

ConfigLoader::ConfigLoader()
{
  if (wxPlatformInfo::Get().GetOperatingSystemFamilyName() == "Windows")
  {
    this->config_file_name += "\\proGramm_config.json";
  }
  else
  {
    this->config_file_name += "/proGramm_config.json";
  }

  std::ifstream input_file;
  
  try
  {
    input_file = std::ifstream(this->config_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file.close();
    }
    else
    {
      this->set_defaults();
    }
  }
  catch(...)
  {
    this->set_defaults();
  }
}

int ConfigLoader::load_int_parameter(std::string identifier)
{
  std::ifstream input_file;

  auto config_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->config_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> config_data;

      input_file.close();
    }
  }
  catch (...)
  {
  }

  try
  {
    return config_data[identifier];
  }
  catch (...)
  {
    std::cerr << "Cannot find identifier: " << identifier << "\n";
    return 0;
  }
}

void ConfigLoader::set_int_parameter(std::string identifier, int value)
{
  auto output_data = nlohmann::json();
  output_data[identifier] = value;

  
  std::ifstream input_file;

  auto old_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->config_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> old_data;

      input_file.close();
    }
    else
    {
    }
  }
  catch (...)
  {
    std::cerr << "Cannot open the file" << this->config_file_name << "\n";
  }

  for (auto& element : old_data.items())
  {
    output_data[element.key()] = element.value();
  }
  
  output_data[identifier] = value;

  std::ofstream output_file(this->config_file_name);
  output_file << output_data << std::endl;
  output_file.close();
}

std::string ConfigLoader::load_string_parameter(std::string identifier)
{
  std::ifstream input_file;

  auto config_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->config_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> config_data;

      input_file.close();
    }
  }
  catch (...)
  {
    std::cerr << "Cannot open the file" << this->config_file_name << "\n";
  }

  return config_data[identifier];
}

void ConfigLoader::set_string_parameter(std::string identifier,
                                        std::string value)
{
  auto output_data = nlohmann::json();
  output_data[identifier] = value;

  std::ifstream input_file;

  auto old_data = nlohmann::json::array();

  try
  {
    input_file = std::ifstream(this->config_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> old_data;

      input_file.close();
    }
    else
    {
    }
  }
  catch (...)
  {
    std::cerr << "Cannot open the file" << this->config_file_name << "\n";
  }

  for (auto& element : old_data.items())
  {
    output_data[element.key()] = element.value();
  }

  output_data[identifier] = value;

  std::ofstream output_file(this->config_file_name);
  output_file << output_data << std::endl;
  output_file.close();
}

nlohmann::json ConfigLoader::get_content()
{
  nlohmann::json output;

  try
  {
    std::ifstream input_file = std::ifstream(this->config_file_name, std::ifstream::in);

    if (input_file.is_open())
    {
      input_file >> output;

      input_file.close();
    }
  }
  catch (...)
  {
    std::cerr << "Cannot open the file" << this->config_file_name << "\n";
  }
  return output;
}


void ConfigLoader::write_json_config(nlohmann::json output_data)
{
  std::ofstream output_file(this->config_file_name);
  output_file << output_data << std::endl;
  output_file.close();
}

void ConfigLoader::set_defaults()
{
  try
  {
    std::ofstream output_file(this->config_file_name);

    // These are some standard values, they can be changed in the GUI
    this->default_values["max_rhs"] = 2;
    this->default_values["execution_time_constant"] = 100;

    // Loading language from config is not yet implemented.
    // Also, the choice should probably be an enumeration
    //this->default_values["Sprache"] = "Deutsch";

    output_file << this->default_values << std::endl;
    output_file.close();
  }
  catch (...)
  {
    std::cerr << "Could not open file to set defaults\n";
  }
}
