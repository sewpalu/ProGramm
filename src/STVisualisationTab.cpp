#include "STVisualisationTab.hpp"

#include <iostream>
#include <memory>

#include "wx/event.h"
#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include "CYKAlgorithm.hpp"
#include "EngineFacade.hpp"
#include "SimpleWordParser.hpp"
#include "StepsDisplay.hpp"
#include "SyntaxTrees.hpp"

FORCE_LINK_ME(STVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(STVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(STVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(STVisualisationTab::on_create)

EVT_SIZE(STVisualisationTab::on_resize)

EVT_SCROLL(STVisualisationTab::sliderMoved)

END_EVENT_TABLE()

void STVisualisationTab::sliderMoved(wxScrollEvent&)
{
  Refresh();
}

// When the mouse has been moved,
void STVisualisationTab::mouseMoved(wxMouseEvent& evt)
{
  if (evt.LeftIsDown())
  {
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

STVisualisationTab::STVisualisationTab()
{
  Show();

  // Binding the "page changed" event here because it doesn't work from the
  // event table
  if (auto* tabs = FindWindowByName("tabs"); tabs)
    dynamic_cast<wxEvtHandler*>(tabs)->Bind(
        wxEVT_NOTEBOOK_PAGE_CHANGED, &STVisualisationTab::on_page_changed,
        this);
}

void STVisualisationTab::draw_tree(const SyntaxTree& tree)
{
  m_tree = std::make_unique<SyntaxTree>(tree);
  m_visualisation_panel->Show();
  Refresh();
}

void STVisualisationTab::draw_empty()
{
  m_tree = nullptr;
  m_visualisation_panel->Show(false);
  m_options_display->Clear();
  m_options_display->Hide();
  m_options_display_label->Hide();
  Refresh();
}

void STVisualisationTab::set_options(const std::vector<Option>& options)
{
  m_options_display->Clear();
  m_options_display->Show();
  m_options_display_label->Show();
  m_option_handlers.clear();

  for (const auto& option : options)
  {
    m_options_display->Append(option.text);
    auto idx = m_options_display->GetCount() - 1;

    if (option.selected)
      m_options_display->SetSelection(idx);

    m_option_handlers.push_back(option.on_click);
  }
}

void STVisualisationTab::render_input()
{
  auto steps = std::vector<StepsDisplay::Step>{
      {.highlight = false, .text = {_("Input (grammar, word)")}},
      {.highlight = false, .text = {_("CNF check")}},
      {.highlight = false,
       .text = {_("CYK")},
       .on_click =
           [this] {
             auto notebook = dynamic_cast<wxNotebook*>(GetParent());
             if (!notebook)
             {
               std::cerr << "ST tab is not a direct child of the notebook\n";
               return;
             }

             auto cyk_tab = notebook->FindWindowByName("cyk_tab");
             if (!cyk_tab)
             {
               std::cerr << "Unable to load CYK tab\n";
               return;
             }
             notebook->SetSelection(notebook->FindPage(cyk_tab));
           }},
      {.highlight = false, .text = {_("Syntax tree")}},
  };
  clear_diagnostics();

  if (!m_current_grammar || !m_current_word)
  {
    draw_empty();
    steps.at(0).highlight = true;
    show_diagnostics(_("Input grammar or input word not set").ToStdString(),
                     DiagnosticsLevel::info);
    m_steps->show_steps(steps);
    return;
  }

  if (auto [is_plausible, why_not] = m_current_grammar->is_plausible();
      !is_plausible)
  {
    draw_empty();
    steps.at(0).highlight = true;
    show_diagnostics(
        _("<b>Grammar plausibility check failed</b>\n<i>Reason</i>\n")
                .ToStdString() +
            why_not,
        DiagnosticsLevel::info);
    m_steps->show_steps(steps);
    return;
  }

  if (auto [is_cnf, why_not] = m_current_grammar->is_cnf(); !is_cnf)
  {
    draw_empty();
    steps.at(1).highlight = true;
    show_diagnostics(
        _("<b>Grammar is not in CNF</b>\n<i>Reason</i>\n").ToStdString() +
            why_not,
        DiagnosticsLevel::warn);
    m_steps->show_steps(steps);
    return;
  }

  // Parse word
  auto engine = EngineFacade{std::make_unique<SimpleWordParser>()};
  engine.setGrammar(*m_current_grammar);
  auto cyk = CYKAlgorithm{};
  auto parse_result = engine.parseWord(cyk, *m_current_word);
  m_visualised_thing = parse_result.empty()
                           ? nullptr
                           : std::make_unique<SyntaxTrees>(parse_result);

  if (auto& cyk_visualiser =
          dynamic_cast<const CYKVisualiser&>(cyk.visualiser());
      !cyk_visualiser.success)
  {
    draw_empty();
    show_diagnostics(
        _("<b>CYK algorithm failed</b>\n<i>Reason</i>\n").ToStdString() +
            cyk_visualiser.error,
        DiagnosticsLevel::error);
  }

  steps.at(3).highlight = true;

  m_steps->show_steps(steps);
  visualise();
}

void STVisualisationTab::on_resize(wxSizeEvent& evt)
{
  this->Refresh();
  evt.Skip();
}

void STVisualisationTab::on_page_changed(wxBookCtrlEvent& evt)
{
  update_visualisation();
  Layout();

  evt.Skip();
}

void STVisualisationTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "st_panel");
  sizer->Add(panel, wxSizerFlags{}.Proportion(1).Expand().Border(wxALL, 5));
  SetSizer(sizer);

  m_zoom_slider =
      dynamic_cast<wxSlider*>(FindWindowByName("st_visualisation_zoom"));
  if (!m_zoom_slider)
  {
    std::cerr << "Failed to load ST zoom slider\n";
    return;
  }
  m_visualisation_panel =
      dynamic_cast<wxPanel*>(FindWindowByName("st_visualisation"));
  if (!m_visualisation_panel)
  {
    std::cerr << "Failed to load ST zoom slider\n";
    return;
  }
  m_visualisation_panel->Bind(wxEVT_PAINT, &STVisualisationTab::on_paint, this);
  m_visualisation_panel->Bind(wxEVT_MOTION, &STVisualisationTab::mouseMoved,
                              this);

  m_steps = dynamic_cast<StepsDisplay*>(FindWindowByName("st_steps"));
  if (!m_steps)
  {
    std::cerr << "Unable to load steps display\n";
    return;
  }

  m_diagnostics =
      dynamic_cast<wxStaticText*>(FindWindowByName("st_diagnostics"));
  if (!m_diagnostics)
  {
    std::cerr << "Unable to load diagnostics display\n";
    return;
  }

  m_options_display =
      dynamic_cast<wxListBox*>(FindWindowByName("st_selection"));
  if (!m_options_display)
  {
    std::cerr << "Unable to load options display in ST tab\n";
    return;
  }
  m_options_display->Bind(
      wxEVT_LISTBOX,
      [this](const wxCommandEvent& evt) {
        auto idx = evt.GetSelection();
        if (idx >= m_option_handlers.size() || idx < 0)
          return;
        auto handler = m_option_handlers.at(static_cast<std::size_t>(idx));
        if (handler)
          handler(*this);
      },
      wxID_ANY);

  m_options_display_label =
      dynamic_cast<wxStaticText*>(FindWindowByName("st_selection_label"));
  if (!m_options_display_label)
  {
    std::cerr << "Unable to load options display label in ST tab\n";
    return;
  }

  update_visualisation();
}

void STVisualisationTab::on_paint(wxPaintEvent&)
{
  if (!m_visualisation_panel)
    return;

  auto dc = wxBufferedPaintDC{m_visualisation_panel};
  if (!m_tree)
  {
    dc.Clear();
    return;
  }

  auto size = m_visualisation_panel->GetSize();
  auto renderer = TreeRenderer(m_tree.get(), dc, size, m_offset,
                               this->m_zoom_slider->GetValue());

  renderer.render();
}
