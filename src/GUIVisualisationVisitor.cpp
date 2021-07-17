#include "GUIVisualisationVisitor.hpp"

#include <map>
#include <algorithm>
#include <cassert>
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
  static constexpr auto skip_to_last_step = true;

  if (visualiser.steps.empty())
    return;

  m_gui.draw_table(to_gui_table(skip_to_last_step ? visualiser.steps.back()
                                                  : visualiser.steps.front()));

  auto idx = std::make_shared<int>(
      skip_to_last_step ? visualiser.steps.size() - 1 : 0);
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
    std::optional<std::array<std::size_t, 3>> selected_cell)
{
  auto result = GUIVisualisationInterface::Table{};

  auto highlighted_symbols = std::vector<std::array<std::size_t, 3>>{};
  if (selected_cell)
    try
    {
      auto productions = cyk_step.at(selected_cell.value().at(0))
                             .at(selected_cell.value().at(1))
                             .at(selected_cell.value().at(2))
                             .getProductions();
      highlighted_symbols.push_back(*selected_cell);
      std::transform(
          productions.begin(), productions.end(),
          std::back_inserter(highlighted_symbols),
          [&cyk_step](const auto& link) -> std::array<std::size_t, 3> {
            // Matrix indices
            auto& [m, n] = link.first;
            auto& candidates = cyk_step.at(m).at(n);
            auto symbol_idx = std::distance(
                candidates.begin(),
                std::find_if(candidates.begin(), candidates.end(),
                             [&link](const auto& elem) {
                               return elem.getRoot() == link.second.getRoot();
                             }));
            assert(symbol_idx >= 0);

            return {m, n, static_cast<unsigned long>(symbol_idx)};
          });
    }
    catch (std::out_of_range)
    {
      highlighted_symbols.clear();
    }

  for (auto y = std::size_t{}; y < cyk_step.size(); ++y)
    for (auto x = std::size_t{}; x < cyk_step.at(y).size(); ++x)
    {
      auto display_coord =
          apply_cyk_layout({x, y}, {cyk_step.size(), cyk_step.front().size()});

      if (cyk_step.at(y).at(x).empty())
      {
        result.push_back({.coord = display_coord});
        continue;
      }

      struct SymbolState
      {
        bool selected;
        bool highlighted;
      };

      auto symbols = std::map<std::string, SymbolState>{};

      for (auto symbol_idx = std::size_t{};
           symbol_idx < cyk_step.at(y).at(x).size(); ++symbol_idx)
      {
        auto name = cyk_step.at(y).at(x).at(symbol_idx).getRoot().identifier;

        auto highlighted =
            std::find(highlighted_symbols.begin(), highlighted_symbols.end(),
                      std::array<std::size_t, 3>{y, x, symbol_idx}) !=
            highlighted_symbols.end();
        auto selected =
            selected_cell && std::array<std::size_t, 3>{y, x, symbol_idx} ==
                                 selected_cell.value();

        symbols[name].highlighted |= highlighted;
        symbols[name].selected |= selected;
      }

      auto text = std::string{};
      auto cell_selected = false;
      auto cell_highlighted = false;
      for (const auto& [name, state]: symbols)
      {
        cell_highlighted |= state.highlighted || state.selected;
        cell_selected |= state.selected;

        auto symbol_string = name;
        if (state.selected)
          symbol_string = "‣" + symbol_string;
        else if (state.highlighted)
          symbol_string = "›" + symbol_string + "‹";
        text += symbol_string + ",";
      }

      if (!text.empty())
        text.pop_back();

      result.push_back(
          {.coord = display_coord,
           .text = text,
           .highlight = cell_highlighted,
           .on_click = [cyk_step, cell_selected, selected_cell, x,
                        y](auto& gui) {
             if (cell_selected)
               gui.draw_table(to_gui_table(
                   cyk_step,
                   cyk_step.at(y).at(x).size() > selected_cell.value()[2] + 1
                       ? std::optional<std::array<
                             std::size_t, 3>>{{y, x,
                                               selected_cell.value()[2] + 1}}
                       : std::optional<std::array<std::size_t, 3>>{}));
             else
               gui.draw_table(to_gui_table(cyk_step, {{y, x, 0}}));
           }});
    }

  return result;
}

namespace
{
enum class CYKLayout
{
  diagonal,
  top,
  bottom
};
}  // namespace

GUIVisualisationInterface::Coord GUIVisualisationVisitor::apply_cyk_layout(
    const GUIVisualisationInterface::Coord& coord,
    const GUIVisualisationInterface::Coord max_coord)
{
  static constexpr auto layout = CYKLayout::diagonal;
  auto [x, y] = coord;

  switch (layout)
  {
  case CYKLayout::diagonal:
    return {y + x, x};
  case CYKLayout::top:
    return {x, y};
  case CYKLayout::bottom:
    return {x, max_coord.y - y};
  }

  return {};
}
