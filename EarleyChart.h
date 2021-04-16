#pragma once

#include <vector>

#include "Production.h"

class EarleyChart
{
public:
	EarleyChart();
	~EarleyChart();

private:
	std::vector<std::pair<Production, unsigned int>> chart;
};

