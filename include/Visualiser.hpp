#ifndef VISUALISER_H
#define VISUALISER_H

#include "VisualisationVisitor.hpp"

class Visualiser
{
public:
  virtual ~Visualiser() = default;

  virtual void accept(VisualisationVisitor& visitor) const = 0;
};

#endif /* VISUALISER_H */
