#include "GUIVisualisationVisitor.hpp"

#include "CYKVisualiser.hpp"
#include "STVisualiser.hpp"
#include <bits/c++config.h>
#include <limits>

GUIVisualisationVisitor::GUIVisualisationVisitor(
    GUIVisualisationInterface& gui_interface)
    : m_gui(gui_interface)
{
}

void GUIVisualisationVisitor::visitCYKVisualiser(
    const CYKVisualiser& visualiser)
{
  auto table = GUIVisualisationInterface::Table{};

  if (visualiser.matrix.empty())
  {
    m_gui.draw_empty();
    return;
  }

  m_gui.draw_table(to_gui_table(visualiser.matrix));

  // TODO: Steps aren't supported by CYKAlgorithm yet ...
  /*
  m_gui.add_button(
      "previous step",
      [steps = visualiser.matrix, idx = 0](auto& gui) mutable {
        if (steps.empty())
          return;
        idx = idx <= 0 ? 0 : (idx - 1);
        gui.draw_table(GUIVisualisationVisitor::to_gui_table(steps.at(idx)));
      },
      GUIVisualisationInterface::Position::left);
  m_gui.add_button(
      "next step",
      [steps = visualiser.matrix, idx = 0](auto& gui) mutable {
        if (steps.empty() || steps.size() > std::numeric_limits<int>::max())
          return;
        idx = idx >= static_cast<int>(steps.size() - 1) ? steps.size() - 1
                                                        : (idx + 1);
        gui.draw_table(GUIVisualisationVisitor::to_gui_table(steps.at(idx)));
      },
      GUIVisualisationInterface::Position::right);
  */
}

void GUIVisualisationVisitor::visitSTVisualiser(const STVisualiser& visualiser)
{
  // TODO
}

GUIVisualisationInterface::Table GUIVisualisationVisitor::to_gui_table(
    const std::vector<std::vector<std::vector<CYKLink>>>& cyk_step)
{
  auto result = GUIVisualisationInterface::Table{};

  for (auto y = std::size_t{}; y < cyk_step.size(); ++y)
    for (auto x = std::size_t{}; x < cyk_step.at(y).size(); ++x)
    {
      auto text = std::string{};
      for (const auto& element : cyk_step.at(y).at(x))
        text += element.getRoot().getIdentifier() + ",";
      if (!text.empty())
        text.pop_back();
      result.push_back({.coord = {x, y}, .text = text});
    }

  return result;
}
