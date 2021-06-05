#include "CYKVisualisationTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>
#include <memory>

#include "CYKAlgorithm.hpp"
#include "EngineFacade.hpp"
#include "SimpleWordParser.hpp"
#include "StepsDisplay.hpp"

FORCE_LINK_ME(CYKVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(CYKVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(CYKVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(CYKVisualisationTab::on_create)
EVT_GRID_CELL_LEFT_CLICK(CYKVisualisationTab::on_grid_click)
EVT_SIZE(CYKVisualisationTab::on_resize)
END_EVENT_TABLE()

CYKVisualisationTab::CYKVisualisationTab()
{
  Show();

  // Binding the "page changed" event here because it doesn't work from the
  // event table
  if (auto* tabs = FindWindowByName("tabs"); tabs)
    dynamic_cast<wxEvtHandler*>(tabs)->Bind(
        wxEVT_NOTEBOOK_PAGE_CHANGED, &CYKVisualisationTab::on_page_changed,
        this);
}

void CYKVisualisationTab::render_input()
{
  auto steps = std::vector<StepsDisplay::Step>{
      {.highlight = false, .text = {"Input (grammar, word)"}},
      {.highlight = false, .text = {"CNF check"}},
      {.highlight = false, .text = {"CYK"}},
      {.highlight = false,
       .text = {"Syntax tree"},
       .on_click =
           [this] {
             auto notebook = dynamic_cast<wxNotebook*>(GetParent());
             if (!notebook)
             {
               std::cerr << "CYK tab is not a direct child of the notebook\n";
               return;
             }

             auto st_tab = notebook->FindWindowByName("st_tab");
             if (!st_tab)
             {
               std::cerr << "Unable to load ST tab\n";
               return;
             }
             notebook->SetSelection(notebook->FindPage(st_tab));
           }},
  };
  clear_diagnostics();

  if (!m_current_grammar || !m_current_word)
  {
    m_table->Show(false);
    steps.at(0).highlight = true;
    show_diagnostics("Input grammar and input word not set",
                     DiagnosticsLevel::info);
    m_steps->show_steps(steps);
    return;
  }

  if (auto [is_plausible, why_not] = m_current_grammar->is_plausible();
      !is_plausible)
  {
    m_table->Show(false);
    steps.at(0).highlight = true;
    show_diagnostics(
        "<b>Grammar plausibility check failed</b>\n<i>Reason</i>\n" + why_not,
        DiagnosticsLevel::warn);
    m_steps->show_steps(steps);
    return;
  }

  if (auto [is_cnf, why_not] = m_current_grammar->is_cnf(); !is_cnf)
  {
    m_table->Show(false);
    steps.at(1).highlight = true;
    show_diagnostics("<b>Grammar is not in CNF</b>\n<i>Reason</i>\n" + why_not,
                     DiagnosticsLevel::warn);
    m_steps->show_steps(steps);
    return;
  }

  // Parse word
  auto engine = EngineFacade{std::make_unique<SimpleWordParser>()};
  engine.setGrammar(*m_current_grammar);
  m_visualised_thing = std::make_unique<CYKAlgorithm>();
  engine.parseWord(dynamic_cast<CYKAlgorithm&>(*m_visualised_thing),
                   *m_current_word);
  steps.at(2).highlight = true;

  if (auto& cyk_visualiser =
          dynamic_cast<const CYKVisualiser&>(m_visualised_thing->visualiser());
      !cyk_visualiser.success)
  {
    show_diagnostics("<b>CYK algorithm failed</b>\n<i>Reason</i>\n" +
                         cyk_visualiser.error,
                     DiagnosticsLevel::error);
  }

  m_steps->show_steps(steps);
  visualise();
}

void CYKVisualisationTab::draw_table(const Table& table)
{
  if (!m_table)
    return;

  if (table.empty())
  {
    draw_empty();
    return;
  }

  Enable();
  m_table->Show();

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
  m_table->ClearGrid();
  m_table->ClearBackground();
  resize_grid(height + 1, width + 1);

  for (const auto& cell : table)
  {
    m_table->SetCellValue(cell.coord.y, cell.coord.x, cell.text);
    if (cell.highlight)
      m_table->SetCellBackgroundColour(cell.coord.y, cell.coord.x, *wxGREEN);
    else
      m_table->SetCellBackgroundColour(
          cell.coord.y, cell.coord.x,
          m_table->GetDefaultCellBackgroundColour());
    if (cell.on_click)
      m_cell_click_handlers[cell.coord] = cell.on_click;
  }

  Layout();
}

void CYKVisualisationTab::draw_empty()
{
  resize_grid(0, 0);
  m_table->Show(false);
}

void CYKVisualisationTab::add_button(const std::string&, Callback on_click,
                                     Position position)
{
  wxButton* button;
  switch (position)
  {
  case Position::left:
    button = m_prev_button;
    break;
  case Position::right:
    button = m_next_button;
    break;
  case Position::centre:
    throw std::runtime_error{
        "CYKVisualisationTab doesn't support centre buttons"};
    break;
  }

  if (!button)
    return;  // Only work with existing buttons for simplicity

  button->Bind(wxEVT_BUTTON, [this, on_click](auto&) { on_click(*this); });
}

void CYKVisualisationTab::on_page_changed(wxBookCtrlEvent& evt)
{
  update_visualisation();
  Layout();

  evt.Skip();
}

void CYKVisualisationTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "cyk_panel");
  sizer->Add(panel, wxSizerFlags{}.Proportion(1).Expand().Border(wxALL, 5));
  SetSizer(sizer);

  m_table = dynamic_cast<wxGrid*>(FindWindowByName("cyk_grid"));
  if (!m_table)
  {
    std::cerr << "Failure loading cyk table\n";
    return;
  }
    m_table->CreateGrid(0, 0, wxGrid::wxGridSelectNone);
    m_table->HideColLabels();
    m_table->HideRowLabels();
    m_table->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
    m_table->AutoSizeColumns();
    m_table->EnableEditing(false);
    m_table->EnableDragGridSize(false);
    m_table->EnableGridLines(true);
    m_table->EnableDragColMove(false);
    m_table->EnableDragColSize(false);
    m_table->EnableDragRowSize(false);
    m_table->SetCellHighlightPenWidth(0);
    m_table->SetCellHighlightROPenWidth(0);

  m_prev_button = dynamic_cast<wxButton*>(FindWindowByName("prev_button"));
  m_next_button = dynamic_cast<wxButton*>(FindWindowByName("next_button"));
  if (!m_prev_button || !m_next_button)
  {
    std::cerr << "Unable to load cyk buttons\n";
    return;
  }

  m_steps = dynamic_cast<StepsDisplay*>(FindWindowByName("cyk_steps"));
  if (!m_steps)
  {
    std::cerr << "Unable to load steps display\n";
    return;
  }

  m_diagnostics =
      dynamic_cast<wxStaticText*>(FindWindowByName("cyk_diagnostics"));
  if (!m_diagnostics)
  {
    std::cerr << "Unable to load diagnostics display\n";
    return;
  }

  update_visualisation();
}

void CYKVisualisationTab::on_resize(wxSizeEvent&)
{
  Refresh();
  Layout();
}

void CYKVisualisationTab::on_grid_click(wxGridEvent& evt)
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
void CYKVisualisationTab::resize_grid(std::size_t n_cols, std::size_t n_rows)
{
  auto current_n_cols = m_table->GetNumberCols();
  auto current_n_rows = m_table->GetNumberRows();

  if (n_cols > current_n_cols)
    m_table->AppendCols(n_cols - current_n_cols);
  else if (n_cols < current_n_cols)
    m_table->DeleteCols(0, current_n_cols - n_cols);

  if (n_rows > current_n_rows)
    m_table->AppendRows(n_rows - current_n_rows);
  else if (n_rows < current_n_rows)
    m_table->DeleteRows(0, current_n_rows - n_rows);

  m_table->ClearSelection();
}
