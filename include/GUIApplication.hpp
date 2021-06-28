#ifndef GUI_APPLICATION_HPP
#define GUI_APPLICATION_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "MainWindow.hpp"

class GUIApplication : public wxApp
{
private:
  MainWindow* m_main_window;
  wxLocale m_locale;

public:
  /**
   * Runs the application
   */
  static void run(int argc, char* argv[]);

private:
  GUIApplication() = default;

  /**
   * Does application wide, GUI related initialisations.
   * E.g., XRC system, splash screen, loading MainWindow
   */
  bool OnInit() final;

  void init_xrc() const;
};

#endif /* ifndef GUI_APPLICATION_HPP */
