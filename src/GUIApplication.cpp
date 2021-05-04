#include "GUIApplication.hpp"

#include <chrono>
#include <memory>
#include <thread>

#include "wx/image.h"
#include "wx/xrc/xmlres.h"

#include "MainWindow.hpp"
#include "SplashScreen.hpp"
#include "TimerDelay.hpp"

void GUIApplication::run()
{
  wxApp::SetInstance(this);
  auto argc = 0;
  auto argv = static_cast<char**>(nullptr);
  wxEntry(argc, argv);
}

bool GUIApplication::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  wxImage::AddHandler(new wxPNGHandler);
  auto splash = SplashScreen{"resources/splash.png", "Gramma"};

  auto delay = TimerDelay::sec(3.14);

  init_xrc();

  auto frame = MainWindow{};

  return true;
}

void GUIApplication::init_xrc() const
{
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->LoadAllFiles("resources/xrc");
}
