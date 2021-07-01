#ifndef VISUALISER_H
#define VISUALISER_H

#include "VisualisationVisitor.hpp"
#include <memory>

class Visualiser
{
public:
  virtual ~Visualiser() = default;

  virtual void accept(VisualisationVisitor& visitor) const = 0;

  virtual std::unique_ptr<Visualiser> clone() const = 0;
};

#endif /* VISUALISER_H */
