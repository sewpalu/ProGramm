#include "text_visualisation_visitor.hpp"

#include <algorithm>

#include "cyk_visualiser.hpp"
#include "st_node.hpp"
#include "st_visualiser.hpp"

#include <iostream>

template <typename ContainerT, typename ElementT>
static ContainerT repeat(const ElementT& item, const std::size_t count)
{
  auto container = ContainerT{};
  auto item_container = ContainerT{item};
  for (auto i = std::size_t{}; i < count; ++i)
    container.insert(container.end(), item_container.begin(),
                     item_container.end());
  return container;
}

static std::string pretty_grid_string(const std::string& begin,
                                      const std::string& mid,
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

void TextVisualisationVisitor::visitCYKVisualiser(
    const CYKVisualiser& visualiser)
{
  using namespace std::literals;
  const auto& matrix = visualiser.matrix();

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
                      .productions
          : decltype(matrix[0][0][0].productions){};

  for (auto m = std::size_t{}; m < height; ++m)
    for (auto n = std::size_t{}; n < width; ++n)
    {
      auto cell = ""s;
      for (const auto& highlighted_cell : highlighted_cells)
        if (highlighted_cell.first == m && highlighted_cell.second == n)
          cell += "* "s;
      for (const auto& symbol : matrix[m][n])
      {
        const auto& id = symbol.root.identifier();
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

static std::string subtree_printer(const STNode& node,
                                   const std::string pre_indentation = {},
                                   const std::string indentation = {},
                                   const bool last = false)
{
  using namespace std::literals;
  auto result =
      (indentation.empty() ? ""s : pre_indentation + (last ? "└──"s : "├──"s)) +
      node.value->identifier() + "\n";
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

void TextVisualisationVisitor::visitSTVisualiser(const STVisualiser& visualiser)
{
  const auto& tree = visualiser.root_node();
  m_text = subtree_printer(tree);
}
