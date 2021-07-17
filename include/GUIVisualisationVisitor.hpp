#ifndef GUI_VISUALISATION_VISITOR_HPP
#define GUI_VISUALISATION_VISITOR_HPP

#include <optional>

#include <array>
#include <cstddef>

#include "CYKLink.hpp"
#include "GUIVisualisationInterface.hpp"
#include "VisualisationVisitor.hpp"

/**
 * Visualisation visitor for GUI visualisation.
 *
 * Has to be provided an object implementing the GUIVisualisationInterface.
 */
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
      std::optional<std::array<std::size_t, 3>> highlighted_cell = {});

  static GUIVisualisationInterface::Coord apply_cyk_layout(
      const GUIVisualisationInterface::Coord& coord,
      const GUIVisualisationInterface::Coord max_coord);
};

#endif /* GUI_VISUALISATION_VISITOR_HPP */
