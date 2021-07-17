#ifndef GUI_VISUALISATION_INTERFACE_HPP
#define GUI_VISUALISATION_INTERFACE_HPP

#include <compare>
#include <functional>
#include <stddef.h>
#include <string>
#include <vector>

#include "STNode.hpp"

#include "SyntaxTree.hpp"

/**
 * Interface for GUI functionality.
 *
 * This serves decoupling the GUI implementation from the Visualisation.
 *
 * Implementors may provide an empty implementation for those member functions
 * that are not expected to be called.
 */
class GUIVisualisationInterface
{
public:
  using Callback = std::function<void(GUIVisualisationInterface&)>;

  struct Tree
  {
    std::vector<Tree> children;
    std::string text;
    Callback on_click;

    STNode root;

    std::size_t n_leaves() const;
    std::size_t n_leaves(std::size_t level) const;
    std::size_t depth() const;

  private:
    std::size_t n_leaves_level_helper(std::size_t wipLevel, std::size_t level,
                                      const Tree& workingNode) const;
    std::size_t depth_helper(const Tree& working_tree,
                             std::size_t acc_depth) const;
  };

  struct Coord
  {
    std::size_t x, y;

    auto operator<=>(const Coord& other) const = default;
  };

  struct Cell
  {
    Coord coord;
    std::string text;
    bool highlight;
    Callback on_click;
  };
  using Table = std::vector<Cell>;

  enum class Position
  {
    left,
    centre,
    right
  };

  struct Option
  {
    std::string text;
    bool selected;
    Callback on_click;
  };

  /**
   * Draws a tree.
   *
   * In the future this should take a GUIVisualisationInterface::Tree instead of
   * a SyntaxTree, in order to allow more generic usage, as well as handling
   * on_click events, etc. .
   */
  virtual void draw_tree(const SyntaxTree& tree) = 0;

  /**
   * Draws a table.
   */
  virtual void draw_table(const Table& table) = 0;

  /**
   * Clears what is currently drawn.
   */
  virtual void draw_empty() = 0;

  /**
   * Assigns a function to the specified button.
   *
   * \param label     New button label.
   * \param on_click  Callback for the click event.
   * \param position  Selects which button to assign a function.
   */
  virtual void set_button(const std::string& label, Callback on_click,
                          Position position = Position::left) = 0;

  /**
   * Provides a list of options for the user to choose from.
   *
   * Selection is handled by the callback specified in the respective Option.
   */
  virtual void set_options(const std::vector<Option>& options) = 0;
};

#endif /* ifndef GUI_VISUALISATION_INTERFACE_HPP */
