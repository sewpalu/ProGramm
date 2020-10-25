#pragma once

#include <vector>
#include <memory>

#include "Symbol.hpp"

struct STNode
{

public:
	std::vector<STNode> children;
        std::unique_ptr<Symbol> value;

	STNode();
	~STNode();
	explicit STNode(std::unique_ptr<Symbol> value_input);
	STNode(std::unique_ptr<Symbol> value_input, std::vector<STNode> children);

	STNode(const STNode& other)
	{
	  *this = other;
	}
	
	STNode& operator=(const STNode& other)
	{
	  children = other.children;
	  value = other.value->clone();
	  return *this;
	}

	void addChildren(std::vector<STNode> children_input);

};

