#include "STVisualisationTab.hpp"

#include <iostream>
#include <memory>

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include "CYKAlgorithm.hpp"
#include "EngineFacade.hpp"
#include "SimpleWordParser.hpp"

FORCE_LINK_ME(STVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(STVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(STVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(STVisualisationTab::on_create)

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

void STVisualisationTab::draw_tree(SyntaxTree* tree)
{
  m_tree.reset(tree);
  Refresh();
}

void STVisualisationTab::draw_empty()
{
  m_tree = nullptr;
  Refresh();
}

void STVisualisationTab::update_input(const FormalGrammar& grammar,
                                      const std::string& word)
{
  auto engine = EngineFacade{std::make_unique<SimpleWordParser>()};
  engine.setGrammar(grammar);

  auto cyk = CYKAlgorithm{};
  auto parse_result = engine.parseWord(cyk, word);
  m_visualised_thing = parse_result.empty()
                           ? nullptr
                           : std::make_unique<SyntaxTree>(parse_result.front());

  visualise();
}

void STVisualisationTab::on_page_changed(wxBookCtrlEvent& evt)
{
  // Don't do anything when we're not changing to this page
  if (auto tabs = dynamic_cast<wxBookCtrlBase*>(evt.GetEventObject());
      tabs && !tabs->GetPage(evt.GetSelection())->IsDescendant(this))
    return;

  Layout();
  update_visualisation();

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
    std::cerr << "Failed to load ST zoom slider\n";

  m_visualisation_panel =
      dynamic_cast<wxPanel*>(FindWindowByName("st_visualisation"));
  if (!m_visualisation_panel)
    std::cerr << "Failed to load ST zoom slider\n";
  else
  {
    m_visualisation_panel->Bind(wxEVT_PAINT, &STVisualisationTab::on_paint,
                                this);
    m_visualisation_panel->Bind(wxEVT_MOTION, &STVisualisationTab::mouseMoved,
                                this);
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

void STVisualisationTab::update_visualisation()
{
  if (m_visualised_thing)
    visualise();
  else
    draw_empty();
}
