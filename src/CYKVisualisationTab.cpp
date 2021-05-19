#include "CYKVisualisationTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>
#include <memory>

#include "CYKAlgorithm.hpp"
#include "EngineFacade.hpp"
#include "SimpleWordParser.hpp"
#include "VisualisationWidget.hpp"

FORCE_LINK_ME(CYKVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(CYKVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(CYKVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(CYKVisualisationTab::on_create)
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

void CYKVisualisationTab::update_input(const FormalGrammar& grammar,
                                       const std::string& word)
{
  auto engine = EngineFacade{std::make_unique<SimpleWordParser>()};

  engine.setGrammar(grammar);

  m_visualised_thing = std::make_unique<CYKAlgorithm>();

  engine.parseWord(dynamic_cast<CYKAlgorithm&>(*m_visualised_thing), word);

  if (!m_visualisation)
  {
    if (auto* visualisation = dynamic_cast<VisualisationWidget*>(
            FindWindowByName("cyk_visualisation"));
        visualisation)
      m_visualisation = visualisation;
    else
      return;
  }
  std::cout << "vis\n";

  visualise();
}

void CYKVisualisationTab::on_page_changed(wxBookCtrlEvent& evt)
{
  // Don't do anything when we're not changing to this page
  if (auto tabs = dynamic_cast<wxBookCtrlBase*>(evt.GetEventObject());
      tabs && !tabs->GetPage(evt.GetSelection())->IsDescendant(this))
    return;

  Layout();
  update_visualisation();

  evt.Skip();
}

void CYKVisualisationTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "cyk_panel");
  sizer->Add(panel, 1, wxEXPAND | wxALL, 5);
  SetSizer(sizer);

  update_visualisation();
}

void CYKVisualisationTab::update_visualisation()
{
  if (auto* visualisation = FindWindowByName("cyk_visualisation");
      visualisation)
    m_visualisation = dynamic_cast<VisualisationWidget*>(visualisation);
  else
    return;

  auto table = VisualisationWidget::Table{
      {.coord = {0, 0},
       .text = "click me",
       .on_click =
           [](auto& vis) {
             vis.draw_table({{.coord = {0, 0}, .text = "empty"}});
           }},
      {.coord = {1, 2}, .text = "hello", .highlight = true},
      {.coord = {2, 1}, .text = "world"}};

  m_visualisation->draw_table(table);
}
