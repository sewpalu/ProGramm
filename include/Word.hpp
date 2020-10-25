#pragma once

#include <vector>

#include "Terminal.hpp"

struct Word
{

public:
	std::vector<Terminal> content;

	unsigned int getSize();

	Terminal at(int position);

	Word();
	explicit Word(std::vector<Terminal> content_input);
	~Word();

};

