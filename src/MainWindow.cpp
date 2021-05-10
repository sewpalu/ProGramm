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
}
