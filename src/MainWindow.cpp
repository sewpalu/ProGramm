#include "MainWindow.hpp"

#include <iostream>

#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include "wx/stdpaths.h"

FORCE_LINK(GrammarEditor);

MainWindow::MainWindow()
{
  if (!wxXmlResource::Get()->LoadFrame(this, nullptr, "main_window"))
  {
    std::cerr << "Failure loading main window frame\n";
    return;
  }

  Show();
}
