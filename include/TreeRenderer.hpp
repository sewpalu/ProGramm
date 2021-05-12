#ifndef TREE_RENDERER_HPP
#define TREE_RENDERER_HPP

#include <vector>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/dc.h"

#include "wx/panel.h"

#include "GUIVisualisationInterface.hpp"

#include "SyntaxTree.hpp"

#include <chrono>

/**
 * Renders a GUIVisualisationInterface::Tree.
 *
 * This is a helper class that should be instantiated for each call to render.
 * If not, make sure that the dc remains valid over the lifetime of
 * TreeRenderer.
 *
 * An idiomatic usage would be `TreeRenderer{my_tree, my_dc, my_size}();`.
 */
class TreeRenderer : public wxPanel
{
public:
  using Tree = GUIVisualisationInterface::Tree;

private:
  const SyntaxTree m_tree;
  wxDC& m_dc;
  const wxSize m_size;
  std::vector<int> m_numberOfNodesOnLevel;
  std::pair<int, int> m_offset = {0, 0};

  int maxStringLength = 1;


  // This is the starting point with a detailed time stamp for dragging the
  // syntax tree
  std::pair<wxPoint, std::chrono::high_resolution_clock::time_point>
      m_dragStartingPoint;


public:
  TreeRenderer(const SyntaxTree& tree, wxDC& dc, wxSize size,
               std::pair<int, int> offset);

  void changeOffset(std::pair<int, int> offset);

  void operator()()
  {
    render();
  }

  void render();

  void updateDimensions(wxSize newSize);
  void updateOffset(std::pair<int, int> offset);

private:
  void render_subtree(STNode subtree, int radius, std::size_t depth,
                      int parentX, int parentY);
  void mouseMoved(wxMouseEvent& evt);

  DECLARE_EVENT_TABLE()
};

#endif /* ifndef TREE_RENDERER_HPP */
