#ifndef GUI_VISUALISATION_INTERFACE_HPP
#define GUI_VISUALISATION_INTERFACE_HPP

#include <functional>
#include <stddef.h>
#include <string>
#include <vector>

#include "STNode.hpp"

#include "SyntaxTree.hpp"

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

    bool operator<(const Coord& other) const;
  };

  struct Cell
  {
    Coord coord;
    std::string text;
    bool highlight;
    Callback on_click;
  };
  using Table = std::vector<Cell>;

  virtual void draw_tree(SyntaxTree* tree) = 0;
  virtual void draw_table(const Table& table) = 0;
  virtual void draw_empty() = 0;

};

#endif /* ifndef GUI_VISUALISATION_INTERFACE_HPP */
