#ifndef ST_VISUALISER_H
#define ST_VISUALISER_H

#include "Visualiser.hpp"
#include <memory>

struct STNode;

class STVisualiser : public Visualiser
{
private:
  const STNode& m_root_node;

public:
  explicit STVisualiser(const STNode& root_node) : m_root_node(root_node)
  {
  }

  const STNode& root_node() const
  {
    return m_root_node;
  }

  void accept(VisualisationVisitor& visitor) const override
  {
    visitor.visitSTVisualiser(*this);
  }

  std::unique_ptr<Visualiser> clone() override
  {
    return std::make_unique<STVisualiser>(*this);
  }
};

#endif /* ST_VISUALISER_H */
