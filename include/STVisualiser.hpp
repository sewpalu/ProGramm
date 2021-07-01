#ifndef ST_VISUALISER_H
#define ST_VISUALISER_H

#include "Visualiser.hpp"
#include <memory>

class STNode;

class STVisualiser : public Visualiser
{
public:
  std::shared_ptr<const STNode> root_node;

  explicit STVisualiser(const std::shared_ptr<const STNode> root_node)
      : root_node(root_node)
  {
  }

  void accept(VisualisationVisitor& visitor) const override
  {
    visitor.visitSTVisualiser(*this);
  }

  std::unique_ptr<Visualiser> clone() const override
  {
    return std::make_unique<STVisualiser>(*this);
  }
};

#endif /* ST_VISUALISER_H */
