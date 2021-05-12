#include "TreeRenderer.hpp"

BEGIN_EVENT_TABLE(TreeRenderer, wxPanel)

// EVT_PAINT(STVisualisationTab::paintEvent)
// EVT_MAGNIFY(STVisualisationTab::magnify)
EVT_MOTION(TreeRenderer::mouseMoved)

//EVT_SIZE(TreeRenderer::reSized)

END_EVENT_TABLE()

// When the mouse has been moved,
void TreeRenderer::mouseMoved(wxMouseEvent& evt)
{
  if (evt.LeftIsDown())
  {
    // Get time since last position entry
    // If last entry is too old, it might not correspond to the current click
    std::chrono::duration<double, std::milli> time_span =
        std::chrono::high_resolution_clock::now() -
        this->m_dragStartingPoint.second;
    if (time_span.count() < 10)
    {
      // Adjust the movement according to the change in mouse position
      this->m_offset.first += evt.GetPosition().x - this->m_dragStartingPoint.first.x;
      this->m_offset.second +=
          evt.GetPosition().y - this->m_dragStartingPoint.first.y;
    }
    this->m_dragStartingPoint.first = evt.GetPosition();
    this->m_dragStartingPoint.second =
        std::chrono::high_resolution_clock::now();
    std::cout << "Offset: " << this->m_offset.first << " | "
              << this->m_offset.second << "\n";
  }
}

TreeRenderer::TreeRenderer(const SyntaxTree& tree, wxDC& dc, wxSize size, std::pair<int, int> offset)
    : m_tree(tree), m_dc(dc), m_size(size), m_offset(offset)
{
  std::cout << "Tree Renderer Constructed\n";
}

void TreeRenderer::render()
{

  // Set background for nodes
  m_dc.SetBrush(wxColor(220, 220, 220));
  // Set dark grey outline for nodes
  m_dc.SetPen(wxPen(wxColor(70, 70, 70), 2));  
  
  std::cout << "Rendering with size: " << this->m_size.x << " | "
            << this->m_size.y << "\n";
  if (m_size.x < 0 || m_size.y < 0)
    return;

  m_dc.SetBackground(wxBrush(wxColor(255, 255, 255)));
  m_dc.Clear();

  Refresh();

  //m_dc.DrawCircle(wxPoint(50, 50), 30);

  int numberOfLeaves = m_tree.getNumberOfLeaves();
  int radius = (m_size.x / numberOfLeaves / 2) * 0.8;
  //std::cout << "Radius: " << radius << "\n";

  int verticalRadius = (m_size.y / m_tree.getRoot().getMaxDepth() / 2) * 0.8;

  if (verticalRadius < radius)
    radius = verticalRadius;

  m_dc.SetBrush(*wxGREY_BRUSH);             // green filling
  m_dc.SetPen(wxPen(wxColor(0, 0, 0), 2));  // 2-pixels-thick black outline

  m_numberOfNodesOnLevel.clear();

  render_subtree(m_tree.getRoot(), radius, 0, 0, 0);
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
      (((m_size.x / (m_tree.getRoot().getNumberOfNodesOnLevel(depth) + 1)) *
        1.25) *
       this->m_numberOfNodesOnLevel.at(depth)) + this->m_offset.first;
  xCoord -= ((m_size.x / (m_tree.getRoot().getNumberOfLeaves() + 1)) * 1.25);
  int yCoord = 1.1 * radius + (depth * (radius * 2 * 1.25)) + this->m_offset.second;

  std::cout << "Node coordinates: " << xCoord << " | " << yCoord << "\n";

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
