#include "MainWindow.hpp"

#include <iostream>

#include "wx/xrc/xmlres.h"

MainWindow::MainWindow()
{
  if (!wxXmlResource::Get()->LoadFrame(this, nullptr, "MyFrame1"))
  {
    std::cerr << "Failure loading frame\n";
    return;
  }
  Show();
}
