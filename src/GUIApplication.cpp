#include "GUIApplication.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "wx/image.h"
#include "wx/xrc/xmlres.h"

#include "MainWindow.hpp"
#include "SplashScreen.hpp"
#include "TimerDelay.hpp"

void GUIApplication::run()
{
  auto app = new GUIApplication{};
  wxApp::SetInstance(app);
  auto argc = 0;
  auto argv = static_cast<char**>(nullptr);
  wxEntry(argc, argv);
}

bool GUIApplication::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  wxImage::AddHandler(new wxPNGHandler{});
  auto splash = SplashScreen{"resources/splash.png", "Gramma"};
  auto delay = TimerDelay::sec(3.14);

  init_xrc();

  m_main_window = new MainWindow{};

  return true;
}

void GUIApplication::init_xrc() const
{
  wxXmlResource::Get()->InitAllHandlers();
  if (!wxXmlResource::Get()->Load("resources/test.xrc"))
  {
    std::cerr << "Failure loading resources\n";
    return;
  }
}
