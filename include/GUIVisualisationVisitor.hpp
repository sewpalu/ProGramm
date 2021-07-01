#ifndef GUI_VISUALISATION_VISITOR_H
#define GUI_VISUALISATION_VISITOR_H

#include <optional>

#include "CYKLink.hpp"
#include "GUIVisualisationInterface.hpp"
#include "VisualisationVisitor.hpp"

class GUIVisualisationVisitor : public VisualisationVisitor
{
private:
  GUIVisualisationInterface& m_gui;

public:
  GUIVisualisationVisitor(GUIVisualisationInterface& gui_interface);

  void visitCYKVisualiser(const CYKVisualiser& visualiser) override;
  void visitSTVisualiser(const STVisualiser& visualiser) override;
  void visitSTreesVisualiser(const STreesVisualiser& visualiser) override;

private:
  static GUIVisualisationInterface::Table to_gui_table(
      const std::vector<std::vector<std::vector<CYKLink>>>& cyk_step,
      std::optional<GUIVisualisationInterface::Coord> highlighted_cell = {});
};

#endif /* GUI_VISUALISATION_VISITOR_H */
