#include "TreeRenderer.hpp"
#include <bits/c++config.h>

TreeRenderer::TreeRenderer(const Tree& tree, wxDC& dc, wxSize size)
    : m_tree(tree), m_dc(dc), m_size(size)
{
}

void TreeRenderer::render()
{
  m_dc.SetBackground(wxBrush(wxColor(255, 255, 255)));
  m_dc.Clear();

  // Refresh();

  int numberOfLeaves = m_tree.n_leaves();
  int radius = (m_size.x / numberOfLeaves / 2) * 0.8;

  int verticalRadius = (m_size.y / m_tree.depth() / 2) * 0.8;

  if (verticalRadius < radius)
    radius = verticalRadius;

  m_dc.SetBrush(*wxGREY_BRUSH);             // green filling
  m_dc.SetPen(wxPen(wxColor(0, 0, 0), 2));  // 2-pixels-thick black outline

  int printedLeaves = 0;

  m_numberOfNodesOnLevel.clear();

  render_subtree(m_tree, printedLeaves, radius, 0);
}

std::size_t TreeRenderer::render_subtree(const Tree& subtree,
                                         std::size_t n_left_leaves, int radius,
                                         std::size_t depth)
{
  if (this->m_numberOfNodesOnLevel.size() <= depth)
    this->m_numberOfNodesOnLevel.push_back(1);

  m_dc.SetPen(wxPen(wxColor(255, 0, 0), 2));  // 2-pixels-thick red outline
  int xCoord =
      1.1 * radius + (((m_size.x / (m_tree.n_leaves(depth) + 1)) * 1.25) *
                      m_numberOfNodesOnLevel.at(depth));
  xCoord -= ((m_size.x / (m_tree.n_leaves() + 1)) * 1.25);
  m_numberOfNodesOnLevel.at(depth) += 1;
  int yCoord = 1.1 * radius + (depth * (radius * 2 * 1.25));
  m_dc.DrawCircle(wxPoint(xCoord, yCoord), radius /* radius */);
  m_dc.DrawText(subtree.text, wxPoint(xCoord - radius * 0.9, yCoord));
  m_dc.SetPen(wxPen(wxColor(0, 0, 0), 2));  // 2-pixels-thick black outline

  depth++;

  if (subtree.children.size() == 0)
    n_left_leaves++;

  for (unsigned int i = 0; i < subtree.children.size(); i++)
  {
    n_left_leaves =
        render_subtree(subtree.children.at(i), n_left_leaves, radius, depth);
  }

  return n_left_leaves;
}
