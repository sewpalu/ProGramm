#ifndef GUI_VISUALISATION_VISITOR_H
#define GUI_VISUALISATION_VISITOR_H

#include "VisualisationVisitor.hpp"

#include "GUIVisualisationInterface.hpp"

class GUIVisualisationVisitor : public VisualisationVisitor
{
private:
  GUIVisualisationInterface& m_gui;

public:
  GUIVisualisationVisitor(GUIVisualisationInterface& gui_interface);

  void visitCYKVisualiser(const CYKVisualiser& visualiser) override;
  void visitSTVisualiser(const STVisualiser& visualiser) override;
};

#endif /* GUI_VISUALISATION_VISITOR_H */
