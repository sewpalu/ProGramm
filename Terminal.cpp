#include "Terminal.h"

Terminal::Terminal(std::string identifier_input, std::string value_input)
{
	this->identifier = identifier_input;
	this->value = value_input;
}

Terminal::Terminal(std::string content)
{
	this->identifier = content;
	this->value = content;
}

Terminal::Terminal()
{
}

Terminal::~Terminal()
{
}
