#include "TextVisualisationVisitor.hpp"

#include <algorithm>
#include <string>

#include "CYKVisualiser.hpp"
#include "Nonterminal.hpp"
#include "STNode.hpp"
#include "STVisualiser.hpp"
#include "STreesVisualiser.hpp"
#include "Terminal.hpp"

namespace
{

/**
 * Concatenates strings and characters
 *
 * \tparam ContainerT
 * \tparam ElementT Needs to be convertible to std::string
 *
 * \param item
 * \param count
 *
 * \return A container containing `count` copies of `item`
 */
template <typename ElementT>
std::string repeat(const ElementT& item, const std::size_t count)
{
  auto container = std::string{};
  for (auto i = std::size_t{}; i < count; ++i)
    container += item;
  return container;
}

/**
 * Puts strings in a sequence of decorators
 *
 * \param begin Beginning decorator
 * \param mid   Interleaved decorator
 * \param end   Ending decorator
 * \param spacings Space between decorators (content)
 *
 * \return The string
 */
std::string pretty_grid_string(const std::string& begin, const std::string& mid,
                               const std::string& end,
                               const std::vector<std::string>& spacings)
{
  auto result = begin;

  if (spacings.size())
  {
    for (const auto& spacing : spacings)
      result += spacing + mid;
    result.resize(result.size() - mid.size());
  }
  result += end;

  return result;
}

/**
 * Prints a syntax tree in a recursive fashion
 *
 * \param node The tree
 * \param pre_indentation String to print as the indentation for the root node
 * \param indentation     String to print as the indentation for child nodes
 * \param last Whether this is the last tree node in the greater context
 *
 * \return The tree as a string
 */
std::string subtree_printer(const STNode& node,
                            const std::string pre_indentation = {},
                            const std::string indentation = {},
                            const bool last = false)
{
  using namespace std::literals;

  auto label = std::string{};
  if (auto* nonterminal = dynamic_cast<Nonterminal*>(node.value.get());
      nonterminal)
    label = nonterminal->identifier;
  else if (auto* terminal = dynamic_cast<Terminal*>(node.value.get()); terminal)
    label = terminal->identifier + "(" + terminal->value + ")";

  auto result =
      (indentation.empty() ? ""s : pre_indentation + (last ? "└──"s : "├──"s)) +
      label + "\n";

  if (node.children.empty())
    return result;

  auto rtail = decltype(node.children){node.children.begin(),
                                       std::prev(node.children.end())};
  for (const auto& child : rtail)
    result += subtree_printer(child, indentation, indentation + "│  "s);

  result += subtree_printer(node.children.back(), indentation,
                            indentation + "   "s, true);

  return result;
}
}  // namespace

void TextVisualisationVisitor::visitCYKVisualiser(
    const CYKVisualiser& visualiser)
{
  using namespace std::literals;
  const auto& matrix = visualiser.matrix;

  if (matrix.empty() || matrix[0].empty())
    return;

  const auto width = matrix[0].size();
  const auto height = matrix.size();

  // Converting into a column first data structure
  // in order to determine the column widths more easily
  auto columns = std::vector<std::vector<std::string>>{};
  columns.resize(width);
  for (auto& column : columns)
    column.resize(height);

  auto highlighted_cells =
      m_highlighted_cell
          ? matrix[m_highlighted_cell->at(0)][m_highlighted_cell->at(1)]
                  [m_highlighted_cell->at(2)]
                      .getProductions()
          : decltype(matrix[0][0][0].getProductions()){};

  for (auto m = std::size_t{}; m < height; ++m)
    for (auto n = std::size_t{}; n < width - m; ++n)
    {
      auto cell = ""s;
      for (const auto& highlighted_cell : highlighted_cells)
        if (highlighted_cell.first.first == m &&
            highlighted_cell.first.second == n)
          cell += "* "s;
      for (const auto& symbol : matrix[m][n])
      {
        const auto& id = symbol.getRoot().identifier;
        cell += std::string{id.begin(), id.end()} + ","s;
      }
      if (cell.size() >= 2)
        cell.resize(cell.size() - 1);  // erase trailing ","

      columns[n][m] = cell;
    }

  // Calculate the column widths: width_i = max({width(cell) | cell ∈ column_i})
  auto column_widths = std::vector<std::size_t>{};
  column_widths.reserve(width);
  std::transform(columns.begin(), columns.end(),
                 std::back_inserter(column_widths), [](const auto& column) {
                   return std::max_element(
                              column.begin(), column.end(),
                              [](const auto& lhs, const auto& rhs) {
                                return lhs.size() < rhs.size();
                              })
                       ->size();
                 });

  // Write to internal state
  auto spacings_bottom_top = std::vector<std::string>{};
  auto spacings_mid = std::vector<std::string>{};
  for (auto width : column_widths)
  {
    spacings_bottom_top.push_back(repeat<std::string>("━", width));
    spacings_mid.push_back(repeat<std::string>("─", width));
  }
  auto grid_top = pretty_grid_string("┏", "┯", "┓\n", spacings_bottom_top);
  auto grid_mid = pretty_grid_string("┠", "┼", "┨\n", spacings_mid);
  auto grid_bot = pretty_grid_string("┗", "┷", "┛\n", spacings_bottom_top);

  auto grid_rows = std::vector<std::string>{};
  for (auto m = std::size_t{}; m < height; ++m)
  {
    auto content = std::vector<std::string>{};
    for (auto n = std::size_t{}; n < width; ++n)
    {
      content.push_back(columns[n][m]);
      content.back().resize(column_widths[n], ' ');
    }
    grid_rows.push_back(pretty_grid_string("┃", "│", "┃\n", content));
  }

  m_text += pretty_grid_string(grid_top, grid_mid, grid_bot, grid_rows);
}

void TextVisualisationVisitor::visitSTVisualiser(const STVisualiser& visualiser)
{
  m_text = subtree_printer(*visualiser.root_node);
}

void TextVisualisationVisitor::visitSTreesVisualiser(
    const STreesVisualiser& visualiser)
{
  m_text = {};
  for (const auto& st : *visualiser.trees)
    m_text += subtree_printer(st.getRoot());
}
