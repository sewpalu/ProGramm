#include "TreeRenderer.hpp"

TreeRenderer::TreeRenderer(SyntaxTree* tree, wxDC& parent, wxSize size,
                           std::pair<int, int> offset, int zoomPercent)
    : m_tree(tree), m_dc(parent), m_size(size), m_offset(offset), m_zoom_percent(zoomPercent)
{
  // Set background for nodes
  this->m_dc.SetBrush(wxColor(220, 220, 220));
  // Set dark grey outline for nodes
  m_dc.SetPen(wxPen(wxColor(70, 70, 70), 2));
  // Set black text colour
  m_dc.SetTextForeground(*wxBLACK);

  this->m_size.x *= (double)this->m_zoom_percent / 100;
  this->m_size.y *= (double)this->m_zoom_percent / 100;

  //std::cout << "Tree Renderer Constructed\n";
}

void TreeRenderer::render()
{  
  m_dc.DrawCircle(wxPoint(50, 50), 10);

  if (m_size.x < 0 || m_size.y < 0)
    return;

  //m_dc.SetBackground(wxBrush(wxColor(255, 255, 255)));
  m_dc.Clear();

  //m_dc.DrawCircle(wxPoint(50, 50), 30);

  int numberOfLeaves = m_tree->getNumberOfLeaves();
  int radius = (m_size.x / numberOfLeaves / 2) * 0.8;
  //std::cout << "Radius: " << radius << "\n";

  int verticalRadius = (m_size.y / m_tree->getRoot().getMaxDepth() / 2) * 0.8;

  if (verticalRadius < radius)
    radius = verticalRadius;

  m_numberOfNodesOnLevel.clear();

  render_subtree(m_tree->getRoot(), radius, 0, 0, 0);
  firstRenderingDone = true;
}

void TreeRenderer::render_subtree(STNode subtree, int radius,
                                  std::size_t depth, int parentX, int parentY)
{
  // If the number of nodes on the current level hasn't been saved yet, do so
  // now
  if (this->m_numberOfNodesOnLevel.size() <= depth)
    this->m_numberOfNodesOnLevel.push_back(1);

  // Calculate the coordinates for the current nodes to be printed
  int xCoord =
      1.1 * radius +
      (((m_size.x / (m_tree->getRoot().getNumberOfNodesOnLevel(depth) + 1)) *
        1.25) *
       this->m_numberOfNodesOnLevel.at(depth)) + this->m_offset.first;
  xCoord -= ((m_size.x / (m_tree->getRoot().getNumberOfLeaves() + 1)) * 1.25);
  int yCoord = 1.1 * radius + (depth * (radius * 2 * 1.25)) + this->m_offset.second;

  // If the parent node coordinates are zero, it's the root, so there's no need
  // to print connections
  if (!(parentX == 0 && parentY == 0))
  {
    // Delta in position from current node to parent node
    int toParentX = parentX - xCoord;
    int toParentY = parentY - yCoord;

    // Get vector from this node's center to parent node's center
    int toParentLength =
        sqrt((toParentX * toParentX) + (toParentY * toParentY));

    // Delta for printing connection to parent node
    int xDelta;
    int yDelta;

    // The toParentLength can be smaller than zero if the user moves the
    // window to be unusably small This would cause a division by 0 which isn't
    // cateched by a try catch block Therefore in that case the delta is just
    // set to 0, because at that size the connections between nodes wouldn't be
    // visible anyway
    if (toParentLength > 0)
    {
      xDelta = (toParentX * (toParentLength - radius) / toParentLength);
      yDelta = (toParentY * (toParentLength - radius) / toParentLength);
    }
    else
    {
      xDelta = 0;
      yDelta = 0;
    }

    // Get the point on the edge of the parent node to which the connection
    // should go
    int parentEdgeX = xCoord + xDelta;
    int parentEdgeY = yCoord + yDelta;

    // Draw the connection to the parent node
    // Dont need to get the edge position of the current node, because the
    // current node is rendered on top later anyway
    m_dc.DrawLine(parentEdgeX, parentEdgeY, xCoord, yCoord);
  }

  // If current node is a leaf, set a red outline
  if (subtree.children.size() == 0)
    m_dc.SetPen(wxPen(wxColor(255, 0, 0), 2));  // 2-pixels-thick red outline

  // Increment the number of nodes on this level to set the correct margins
  this->m_numberOfNodesOnLevel.at(depth)++;
  // Draw the circle for the current node
  m_dc.DrawCircle(wxPoint(xCoord, yCoord), radius);

  // If the current string length is larger than the previous maximum, update it
  if (subtree.value->getIdentifier().length() > this->maxStringLength)
    this->maxStringLength = subtree.value->getIdentifier().length();

  // Draw the label for the current node
  m_dc.DrawLabel(
      subtree.value->getIdentifier(),
      wxRect(wxPoint(xCoord - (radius / sqrt(2)),
                     yCoord - (radius / sqrt(2) / this->maxStringLength)),
             wxSize(radius * sqrt(2), radius * sqrt(2))));

  // Set the outline back to black for the next node
  m_dc.SetPen(wxPen(wxColor(70, 70, 70), 2));  // 2-pixels-thick black outline

  // Increment the depth for child nodes
  depth++;

  // Call the function to print all child nodes recursively
  for (unsigned int i = 0; i < subtree.children.size(); i++)
  {
    render_subtree(subtree.children.at(i), radius, depth, xCoord,
                  yCoord);
  }
}

void TreeRenderer::updateDimensions(wxSize newSize)
{
  this->m_size = newSize;
}

void TreeRenderer::updateOffset(std::pair<int, int> offset)
{
  this->m_offset = offset;
}

void TreeRenderer::updateTree(SyntaxTree* tree)
{
  this->m_tree = tree;
}
