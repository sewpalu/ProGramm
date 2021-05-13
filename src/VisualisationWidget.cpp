#include "VisualisationWidget.hpp"

#include <algorithm>

#include "wx/dcbuffer.h"
#include "wx/html/forcelnk.h"

#include "TreeRenderer.hpp"

FORCE_LINK_ME(VisualisationWidget);

wxIMPLEMENT_DYNAMIC_CLASS(VisualisationWidget, wxPanel);

BEGIN_EVENT_TABLE(VisualisationWidget, wxPanel)
EVT_PAINT(VisualisationWidget::on_paint)
EVT_GRID_CELL_LEFT_CLICK(VisualisationWidget::on_grid_click)
EVT_GRID_SELECT_CELL(VisualisationWidget::on_grid_click)
EVT_SIZE(VisualisationWidget::on_resize)
EVT_MOTION(VisualisationWidget::mouseMoved)
EVT_SCROLL(VisualisationWidget::sliderMoved) 

END_EVENT_TABLE()

void VisualisationWidget::sliderMoved(wxScrollEvent& evt)
{
  Refresh();
}

// When the mouse has been moved,
void VisualisationWidget::mouseMoved(wxMouseEvent& evt)
{
  if (evt.LeftIsDown())
  {
    std::cout << "Dragging: " << evt.m_x << " | " << evt.m_y << "\n";

    // Get time since last position entry
    // If last entry is too old, it might not correspond to the current click
    std::chrono::duration<double, std::milli> time_span =
        std::chrono::high_resolution_clock::now() -
        this->dragStartingPoint.second;
    if (time_span.count() < 100)
    {
      // Adjust the movement according to the change in mouse position
      this->m_offset.first += evt.GetPosition().x - dragStartingPoint.first.x;
      this->m_offset.second += evt.GetPosition().y - dragStartingPoint.first.y;
    }
    this->dragStartingPoint.first = evt.GetPosition();
    this->dragStartingPoint.second = std::chrono::high_resolution_clock::now();
    Refresh();
  }
}

VisualisationWidget::VisualisationWidget()
{
  std::cout << "Constructing widget \n";
  draw_empty();
  Show();
}

void VisualisationWidget::draw_tree(SyntaxTree* tree)
{
  reset();

  std::cout << "Zoom slider window: " << this->GetSize().x << " | "
            << this->GetSize().y << "\n";
  this->m_zoom_slider =
      new wxSlider(this, wxID_ANY, 100, 50, 500,
                   wxPoint(0.6 * this->GetSize().x,
                           this->GetSize().y - 40),
                   wxSize(0.4 * this->GetSize().x, 40),
                   wxSL_LABELS, wxDefaultValidator, wxString("Zoom Factor"));

  m_dynamic_paint = [this, tree]() {
    auto dc = wxBufferedPaintDC{this};
    this->m_tree_renderer = new TreeRenderer(tree, dc, GetSize(), m_offset, this->m_zoom_slider->GetValue());
    this->m_zoom_slider->SetPosition(
        wxPoint(0.6 * this->GetSize().x, this->GetSize().y - 40));
    this->m_zoom_slider->SetSize(wxSize(0.4 * this->GetSize().x, 40));
    /*if (this->m_tree_renderer == nullptr)
    {
      std::cout << "Making new tree renderer\n";
    }
    else  
    {
      std::cout << "Updating old tree renderer\n";
      this->m_tree_renderer->updateDimensions(GetSize());
      this->m_tree_renderer->updateOffset(m_offset);
    }*/
    
    std::cout << "Starting render\n";
    m_tree_renderer->render();
    std::cout << "Finished render\n";
  };
}

void VisualisationWidget::draw_table(const Table& table)
{
  reset();

  generate_grid(table);

  auto sizer = new wxBoxSizer{wxVERTICAL};
  sizer->AddStretchSpacer();
  sizer->Add(m_table, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
  sizer->AddStretchSpacer();
  SetSizer(sizer);

  Layout();
}

void VisualisationWidget::draw_empty()
{
  reset();

  m_dynamic_paint = [this]() {
    auto dc = wxBufferedPaintDC{this};
    dc.Clear();
    Refresh();
  };
}

void VisualisationWidget::on_paint(wxPaintEvent&)
{
  if (m_dynamic_paint)
    m_dynamic_paint();
}

void VisualisationWidget::on_grid_click(wxGridEvent& evt)
{
  auto x = evt.GetCol();
  auto y = evt.GetRow();

  if (x < 0 || y < 0)
    return;

  auto handler = m_cell_click_handlers[{.x = static_cast<std::size_t>(x),
                                        .y = static_cast<std::size_t>(y)}];

  if (handler)
    handler(*this);
}

void VisualisationWidget::on_resize(wxSizeEvent& evt)
{
  Refresh();
  Layout();
}

void VisualisationWidget::generate_grid(const Table& table)
{
  m_table = new wxGrid{this, wxID_ANY};
  if (table.empty())
    return;

  auto height = std::max_element(table.begin(), table.end(),
                                 [](const auto& lhs, const auto& rhs) {
                                   return lhs.coord.y < rhs.coord.y;
                                 })
                    ->coord.y;
  auto width = std::max_element(table.begin(), table.end(),
                                [](const auto& lhs, const auto& rhs) {
                                  return lhs.coord.x < rhs.coord.x;
                                })
                   ->coord.x;
  m_table->CreateGrid(height + 1, width + 1, wxGrid::wxGridSelectNone);
  m_table->HideColLabels();
  m_table->HideRowLabels();
  m_table->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

  for (const auto& cell : table)
  {
    m_table->SetCellValue(cell.coord.y, cell.coord.x, cell.text);
    if (cell.highlight)
      m_table->SetCellBackgroundColour(cell.coord.y, cell.coord.x, *wxGREEN);
    if (cell.on_click)
      m_cell_click_handlers[cell.coord] = cell.on_click;
  }

  m_table->AutoSizeColumns();
  m_table->EnableEditing(false);
  m_table->EnableDragGridSize(false);
  m_table->EnableGridLines(true);
  m_table->EnableDragColMove(false);
  m_table->EnableDragColSize(false);
  m_table->EnableDragRowSize(false);
}

void VisualisationWidget::reset()
{
  m_dynamic_paint = {};
  if (m_table)
  {
    m_table->DeleteCols(0, m_table->GetNumberCols());
    m_table->DeleteRows(0, m_table->GetNumberRows());
    m_table->Destroy();
    RemoveChild(m_table);

    m_table = nullptr;
  }
  SetSizer(nullptr);
}
