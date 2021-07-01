#ifndef VISUALISATION_VISITOR_H
#define VISUALISATION_VISITOR_H

class CYKVisualiser;
class STVisualiser;
class STreesVisualiser;

class VisualisationVisitor
{
public:
  virtual ~VisualisationVisitor() = default;

  virtual void visitCYKVisualiser(const CYKVisualiser& visualiser) = 0;
  virtual void visitSTVisualiser(const STVisualiser& visualiser) = 0;
  virtual void visitSTreesVisualiser(const STreesVisualiser& visualiser) = 0;
};

#endif /* VISUALISATION_VISITOR_H */
