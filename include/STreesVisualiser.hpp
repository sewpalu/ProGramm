#ifndef S_TREES_VISUALISER_HPP
#define S_TREES_VISUALISER_HPP

#include <memory>
#include <vector>

#include <SyntaxTree.hpp>

/**
 * Visualiser for multiple SyntaxTrees
 *
 * Needed for the visualisation visitor pattern.
 */
class STreesVisualiser : public Visualiser
{
public:
  using Trees = std::vector<SyntaxTree>;
  std::shared_ptr<const Trees> trees;

  explicit STreesVisualiser(const std::shared_ptr<const Trees> trees)
      : trees(trees)
  {
  }

  void accept(VisualisationVisitor& visitor) const override
  {
    visitor.visitSTreesVisualiser(*this);
  }

  std::unique_ptr<Visualiser> clone() const override
  {
    return std::make_unique<STreesVisualiser>(*this);
  }
};

#endif /* ifndef S_TREES_VISUALISER_HPP */

