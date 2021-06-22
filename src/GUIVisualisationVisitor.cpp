#include "GUIVisualisationVisitor.hpp"

#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>

#include "CYKVisualiser.hpp"
#include "STVisualiser.hpp"
#include "STreesVisualiser.hpp"

GUIVisualisationVisitor::GUIVisualisationVisitor(
    GUIVisualisationInterface& gui_interface)
    : m_gui(gui_interface)
{
}

void GUIVisualisationVisitor::visitCYKVisualiser(
    const CYKVisualiser& visualiser)
{
  if (visualiser.steps.empty())
    return;

  m_gui.draw_table(to_gui_table(visualiser.steps.front()));

  auto idx = std::make_shared<int>(0);
  m_gui.set_button(
      "previous step",
      [steps = visualiser.steps, idx](auto& gui) mutable {
        if (steps.empty() || steps.size() > std::numeric_limits<int>::max())
          return;
        *idx = *idx <= 0 ? static_cast<int>(steps.size()) - 1 : (*idx - 1);
        gui.draw_table(GUIVisualisationVisitor::to_gui_table(steps.at(*idx)));
      },
      GUIVisualisationInterface::Position::left);
  m_gui.set_button(
      "next step",
      [steps = visualiser.steps, idx](auto& gui) mutable {
        if (steps.empty() || steps.size() > std::numeric_limits<int>::max())
          return;
        *idx = *idx >= static_cast<int>(steps.size() - 1) ? 0 : (*idx + 1);
        gui.draw_table(GUIVisualisationVisitor::to_gui_table(steps.at(*idx)));
      },
      GUIVisualisationInterface::Position::right);
}

void GUIVisualisationVisitor::visitSTVisualiser(const STVisualiser& visualiser)
{
  m_gui.draw_empty();
  m_gui.draw_tree(SyntaxTree{*visualiser.root_node});
}

void GUIVisualisationVisitor::visitSTreesVisualiser(
    const STreesVisualiser& visualiser)
{
  auto options = std::vector<GUIVisualisationInterface::Option>{};

  for (auto idx = 0; const auto& tree : *visualiser.trees)
  {
    options.push_back({.text = std::to_string(idx),
                       .selected = (idx == 0),
                       .on_click = [tree](auto& gui) { gui.draw_tree(tree); }});

    if (idx == 0)
    {
      m_gui.draw_tree(tree);
    }
    ++idx;
  }

  m_gui.set_options(options);
}

GUIVisualisationInterface::Table GUIVisualisationVisitor::to_gui_table(
    const std::vector<std::vector<std::vector<CYKLink>>>& cyk_step,
    std::optional<GUIVisualisationInterface::Coord> selected_cell)
{
  auto result = GUIVisualisationInterface::Table{};

  auto highlighted_cells = std::vector<GUIVisualisationInterface::Coord>();
  if (selected_cell)
    try
    {
      auto productions = cyk_step.at(selected_cell->y)
                             .at(selected_cell->x)
                             .at(0)  // TODO: Make individual options selectable
                             .getProductions();
      highlighted_cells.push_back(selected_cell.value());
      std::transform(productions.begin(), productions.end(),
                     std::back_inserter(highlighted_cells),
                     [](const auto& link) -> GUIVisualisationInterface::Coord {
                       return {link.first.second, link.first.first};
                     });
    }
    catch (std::out_of_range)
    {
      highlighted_cells.clear();
    }

  for (auto y = std::size_t{}; y < cyk_step.size(); ++y)
    for (auto x = std::size_t{}; x < cyk_step.at(y).size(); ++x)
    {
      if (cyk_step.at(y).at(x).empty())
      {
        result.push_back({.coord = {x, y}});
        continue;
      }

      auto highlighted =
          std::find(highlighted_cells.begin(), highlighted_cells.end(),
                    GUIVisualisationInterface::Coord{x, y}) !=
          highlighted_cells.end();
      auto selected = selected_cell &&
                      GUIVisualisationInterface::Coord{x, y} == selected_cell;

      auto text = selected ? std::string{"* "} : std::string{};
      for (const auto& element : cyk_step.at(y).at(x))
        text += element.getRoot().getIdentifier() + ",";
      if (!text.empty())
        text.pop_back();
      result.push_back({.coord = {x, y},
                        .text = text,
                        .highlight = highlighted,
                        .on_click = [cyk_step, selected, x, y](auto& gui) {
                          if (selected)
                            gui.draw_table(to_gui_table(cyk_step));
                          else
                            gui.draw_table(to_gui_table(cyk_step, {{x, y}}));
                        }});
    }

  return result;
}
