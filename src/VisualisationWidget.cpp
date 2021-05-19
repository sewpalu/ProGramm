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
END_EVENT_TABLE()

VisualisationWidget::VisualisationWidget()
{
  draw_empty();
  Show();
}

void VisualisationWidget::draw_tree(const Tree& tree)
{
  reset();

  m_dynamic_paint = [this, tree]() {
    auto dc = wxBufferedPaintDC{this};
    TreeRenderer{tree, dc, GetSize()}();
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

void VisualisationWidget::add_button(const std::string& label,
                                     Callback on_click, Position position)
{
  // TODO
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
