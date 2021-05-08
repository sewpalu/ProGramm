#include "MainWindow.hpp"

#include <iostream>

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include "VisualisationWidget.hpp"

FORCE_LINK(GrammarEditor);
FORCE_LINK(CYKVisualisationTab);
FORCE_LINK(STVisualisationTab);
FORCE_LINK(ManagerTab);
FORCE_LINK(VisualisationWidget);
FORCE_LINK(StepsDisplay);

MainWindow::MainWindow()
{
  if (!wxXmlResource::Get()->LoadFrame(this, nullptr, "main_window"))
  {
    std::cerr << "Failure loading main window frame\n";
    return;
  }

  Show();

  auto* visualisation =
      dynamic_cast<VisualisationWidget*>(FindWindowByName("st_visualisation"));
  if (!visualisation)
  {
    std::cerr << "Failure getting VisualisationWidget\n";
    return;
  }

  auto tree =
      VisualisationWidget::Tree{.children = {{.text = "leaf"},
                                             {.children = {{.text = "leaf_a"},
                                                           {.text = "leaf_b"},
                                                           {.text = "leaf_c"}},
                                              .text = "node"}},
                                .text = "root"};

  visualisation->draw_tree(tree);
}
