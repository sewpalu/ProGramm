#include "STVisualisationTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

#include "VisualisationWidget.hpp"

FORCE_LINK_ME(STVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(STVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(STVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(STVisualisationTab::on_create)
END_EVENT_TABLE()

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

void STVisualisationTab::update_input(const FormalGrammar& grammar,
                                      const std::string& word)
{
  // TODO
}

void STVisualisationTab::on_page_changed(wxBookCtrlEvent& evt)
{
  // Don't do anything when we're not changing to this page
  if (auto tabs = dynamic_cast<wxBookCtrlBase*>(evt.GetEventObject());
      tabs && !tabs->GetPage(evt.GetSelection())->IsDescendant(this))
    return;

  Layout();
  update_visualisation();
}

void STVisualisationTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "st_panel");
  sizer->Add(panel, 1, wxEXPAND | wxALL, 5);
  SetSizer(sizer);

  update_visualisation();
}

void STVisualisationTab::update_visualisation()
{
  if (auto* visualisation = FindWindowByName("st_visualisation"); visualisation)
    m_visualisation = dynamic_cast<VisualisationWidget*>(visualisation);
  else
    return;

  auto tree =
      VisualisationWidget::Tree{.children = {{.text = "leaf"},
                                             {.children = {{.text = "leaf_a"},
                                                           {.text = "leaf_b"},
                                                           {.text = "leaf_c"}},
                                              .text = "node"}},
                                .text = "root"};

  m_visualisation->draw_tree(tree);
}
