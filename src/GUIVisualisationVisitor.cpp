#include "GUIVisualisationVisitor.hpp"

#include "CYKVisualiser.hpp"
#include "STVisualiser.hpp"

GUIVisualisationVisitor::GUIVisualisationVisitor(
    GUIVisualisationInterface& gui_interface)
    : m_gui(gui_interface)
{
}

void GUIVisualisationVisitor::visitCYKVisualiser(const CYKVisualiser& visualiser)
{
  // TODO
}

void GUIVisualisationVisitor::visitSTVisualiser(const STVisualiser& visualiser)
{
  // TODO
}
