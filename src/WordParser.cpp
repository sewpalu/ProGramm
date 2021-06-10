#include "WordParser.hpp"
#include <memory>

WordParser::WordParser(std::unique_ptr<Visualiser> visualiser)
  : Visualisable(std::move(visualiser))
{
}

WordParser::~WordParser()
{
}
