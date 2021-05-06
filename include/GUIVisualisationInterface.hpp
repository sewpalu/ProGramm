#ifndef GUI_VISUALISATION_INTERFACE_HPP
#define GUI_VISUALISATION_INTERFACE_HPP

#include <stddef.h>
#include <vector>
#include <functional>
#include <string>

class GUIVisualisationInterface
{
public:
  using Callback = std::function<void(const GUIVisualisationInterface&)>;

  struct Tree
  {
    std::vector<Tree> children;
    std::string text;
    Callback on_click;
  };

  struct Coord
  {
    std::size_t x, y;
  };

  struct Cell
  {
    Coord coord;
    std::string text;
    bool highlight;
    Callback on_click;
  };
  using Table = std::vector<Cell>;

  virtual void draw_tree(const Tree& tree) = 0;
  virtual void draw_table(const Table& table) = 0;
};

#endif /* ifndef GUI_VISUALISATION_INTERFACE_HPP */
