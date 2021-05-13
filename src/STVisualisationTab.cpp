#include "STVisualisationTab.hpp"

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>

#include "VisualisationWidget.hpp"

FORCE_LINK_ME(STVisualisationTab);

wxIMPLEMENT_DYNAMIC_CLASS(STVisualisationTab, wxPanel);

BEGIN_EVENT_TABLE(STVisualisationTab, wxPanel)
EVT_WINDOW_CREATE(STVisualisationTab::on_create)

EVT_MOTION(STVisualisationTab::mouseMoved)


END_EVENT_TABLE()

// When the mouse has been moved,
void STVisualisationTab::mouseMoved(wxMouseEvent& evt)
{
  std::cout << "ST Mouse event\n";
  /*if (evt.LeftIsDown())
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
  }*/
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

void STVisualisationTab::update_input(const FormalGrammar& grammar,
                                      const Word& word)
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

  STNode leaf1(std::unique_ptr<Terminal>(new Terminal("leaf1", "leaf1")));
  STNode leaf2(std::unique_ptr<Terminal>(new Terminal("leaf2", "leaf2")));
  STNode leaf3(std::unique_ptr<Terminal>(new Terminal("leaf3", "leaf3")));
  STNode leaf4(std::unique_ptr<Terminal>(new Terminal("leaf4", "leaf4")));

  STNode node1(std::unique_ptr<Nonterminal>(new Nonterminal("node1")),
               {leaf1, leaf2});
  STNode node2(std::unique_ptr<Nonterminal>(new Nonterminal("node2")),
               {leaf3, leaf4});

  STNode treeRoot(std::unique_ptr<Nonterminal>(new Nonterminal("root")),
               {node1, node2});


  SyntaxTree* testTree = new SyntaxTree(treeRoot);

  /*auto tree =
      VisualisationWidget::Tree{.children = {{.text = "leaf"},
                                             {.children = {{.text = "leaf_a"},
                                                           {.text = "leaf_b"},
                                                           {.text = "leaf_c"}},
                                              .text = "node"}},
                                .text = "root",
                                .root = treeRoot};*/

  m_visualisation->draw_tree(testTree);
}
