#pragma once

#include <memory>
#include <vector>

#include "STNode.hpp"
#include "STVisualiser.hpp"
#include "Visualisable.hpp"

/**
 * Wrapper class for the root STNode.
 */
class SyntaxTree : public Visualisable
{

private:
  const std::shared_ptr<const STNode> root;

public:
  explicit SyntaxTree(STNode root_input);

  const STNode& getRoot() const;

  int getNumberOfLeaves() const;
};
