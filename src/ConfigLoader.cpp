#include "ConfigLoader.hpp"

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
    else
    {
      std::cout << "Not open!\n";
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->config_file_name << "\n";
  }

  std::cout << "Loaded parameter: " << identifier << " with value "
            << config_data[identifier] << "\n";

  return config_data[identifier];
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
      std::cout << "Not open!\n";
      // old_data
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->config_file_name << "\n";
  }

  for (auto& element : old_data.items())
  {
    std::cout << "key: " << element.key() << ", value:" << element.value()
              << '\n';
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
    else
    {
      std::cout << "Not open!\n";
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->config_file_name << "\n";
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
      std::cout << "Not open!\n";
      // old_data
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->config_file_name << "\n";
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
    else
    {
      std::cout << "Not open!\n";
      // old_data
    }
  }
  catch (...)
  {
    std::cout << "Cannot open the file" << this->config_file_name << "\n";
  }
  return output;
}