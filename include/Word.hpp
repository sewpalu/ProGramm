#pragma once

#include <vector>
#include <string>

#include "Terminal.h"

class Word
{

public:
	std::vector<Terminal> content;

	unsigned int getSize();

	Terminal at(int position);

	Word();
	Word(std::vector<Terminal> content_input);
	~Word();

};

