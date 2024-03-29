#ifndef VISUALISABLE_H
#define VISUALISABLE_H

#include <memory>

#include "Visualiser.hpp"

/**
 * Abstract base class signifying that the inheriting class is visualisable.
 *
 * Has to be provided with a visualiser.
 */
class Visualisable
{
protected:
  std::unique_ptr<Visualiser> m_visualiser;

public:
  virtual ~Visualisable() = default;
  Visualisable(const Visualisable& other)
      : m_visualiser(other.m_visualiser->clone())
  {
  }

  const Visualiser& visualiser() const
  {
    return *m_visualiser;
  }

protected:
  explicit Visualisable(std::unique_ptr<Visualiser> visualiser)
      : m_visualiser(std::move(visualiser))
  {
  }
};

#endif /* VISUALISABLE_H */
