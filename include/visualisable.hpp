#ifndef VISUALISABLE_H
#define VISUALISABLE_H

#include <memory>

#include "visualiser.hpp"

class Visualisable
{
protected:
  std::unique_ptr<Visualiser> m_visualiser;

public:
  virtual ~Visualisable() = default;

  const Visualiser& visualiser() const
  {
    return *m_visualiser;
  }

protected:
  Visualisable(std::unique_ptr<Visualiser> visualiser)
      : m_visualiser(std::move(visualiser))
  {
  }
};

#endif /* VISUALISABLE_H */
