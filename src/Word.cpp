#include "Word.hpp"

std::size_t Word::getSize()
{
	return this->content.size();
}

Terminal Word::at(int position)
{
	return this->content.at(position);
}

Word::Word()
{
}

Word::Word(std::vector<Terminal> content_input)
{
	this->content = content_input;
}

Word::~Word()
{
}
