#ifndef TEXT_VISUALISATION_VISITOR_H
#define TEXT_VISUALISATION_VISITOR_H

#include <array>
#include <optional>
#include <string>

#include "visualisation_visitor.hpp"

class TextVisualisationVisitor : public VisualisationVisitor
{
public:
  using Coord = std::array<std::size_t, 3>;

private:
  std::string m_text;
  std::optional<Coord> m_highlighted_cell = {{0, 0, 0}};

public:
  void visitCYKVisualiser(const CYKVisualiser& visualiser) override;
  void visitSTVisualiser(const STVisualiser& visualiser) override;

  void setHighlighted(const Coord cell)
  {
    m_highlighted_cell = cell;
  }

  std::string toString()
  {
    return m_text;
  }
};

#endif /* TEXT_VISUALISATION_VISITOR_H */
