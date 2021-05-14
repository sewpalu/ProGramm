#include "GUIApplication.hpp"

#include <iostream>

#include "wx/image.h"
#include "wx/init.h"
#include "wx/xrc/xmlres.h"

#include "MainWindow.hpp"
#include "SplashScreen.hpp"
#include "TimerDelay.hpp"

void GUIApplication::run(int argc, char* argv[])
{
  auto app = new GUIApplication{};
  wxApp::SetInstance(app);

  if (!wxInitialize(argc, argv))
  {
    std::cerr << "Error initialising wxWidgets\n";
    return;
  }

  if (auto err = wxEntry(argc, argv); err)
    std::cerr << "Error starting GUI (" << err << ")\n";

  wxUninitialize();
}

bool GUIApplication::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  wxImage::AddHandler(new wxPNGHandler{});
  //auto splash = SplashScreen{"resources/splash.png", "Gramma"};
  //auto delay = TimerDelay::sec(3.14);

  init_xrc();

  m_main_window = new MainWindow{};

  return true;
}

void GUIApplication::init_xrc() const
{
  wxXmlResource::Get()->InitAllHandlers();
  if (!wxXmlResource::Get()->Load("resources/gui.xrc"))
  {
    std::cerr << "Failure loading resources\n";
    return;
  }
}
