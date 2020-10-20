#ifndef TEXT_VISUALISATION_VISITOR_H
#define TEXT_VISUALISATION_VISITOR_H

#include <string>

#include "visualisation_visitor.hpp"

class TextVisualisationVisitor : public VisualisationVisitor
{
private:
  std::string m_text;

public:
  void visitCYKVisualiser(const CYKVisualiser& visualiser) override;
  void visitSTVisualiser(const STVisualiser& visualiser) override;

  std::string toString()
  {
    return m_text;
  }
};

#endif /* TEXT_VISUALISATION_VISITOR_H */
