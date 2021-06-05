#include "MainWindow.hpp"

#include <iostream>

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

// Link all dynamically loaded classes
FORCE_LINK(GrammarEditor);
FORCE_LINK(CYKVisualisationTab);
FORCE_LINK(STVisualisationTab);
FORCE_LINK(ManagerTab);
FORCE_LINK(AlphabetManager);
FORCE_LINK(ProductionManager);
FORCE_LINK(GrammarOverviewTab);
FORCE_LINK(AlphabetDisplay);
FORCE_LINK(ProductionDisplay)
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
