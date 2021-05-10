#ifndef TREE_RENDERER_HPP
#define TREE_RENDERER_HPP

#include <vector>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/dc.h"

#include "GUIVisualisationInterface.hpp"

/**
 * Renders a GUIVisualisationInterface::Tree.
 *
 * This is a helper class that should be instantiated for each call to render.
 * If not, make sure that the dc remains valid over the lifetime of
 * TreeRenderer.
 *
 * An idiomatic usage would be `TreeRenderer{my_tree, my_dc, my_size}();`.
 */
class TreeRenderer
{
public:
  using Tree = GUIVisualisationInterface::Tree;

private:
  const Tree m_tree;
  wxDC& m_dc;
  const wxSize m_size;
  std::vector<int> m_numberOfNodesOnLevel;

public:
  TreeRenderer(const Tree& tree, wxDC& dc, wxSize size);

  void operator()()
  {
    render();
  }

  void render();

private:
  std::size_t render_subtree(const Tree& subtree, std::size_t n_left_leaves,
                             int radius, std::size_t depth);
};

#endif /* ifndef TREE_RENDERER_HPP */
