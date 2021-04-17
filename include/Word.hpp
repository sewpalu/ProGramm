#pragma once

#include <vector>

#include "Terminal.hpp"

struct Word
{

public:
	std::vector<Terminal> content;

	std::size_t getSize();

	Terminal at(int position);

	Word();
	explicit Word(std::vector<Terminal> content_input);
	~Word();

};

