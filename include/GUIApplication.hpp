#ifndef GUI_APPLICATION_HPP
#define GUI_APPLICATION_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class GUIApplication : public wxApp
{
public:
  /**
   * Runs the application
   */
  void run();

private:
  /**
   * Does application wide initialisations, that regard to the GUI.
   * E.g. XRC system, splash screen, loading MainWindow
   */
  bool OnInit() final;

  void init_xrc() const;
};

#endif /* ifndef GUI_APPLICATION_HPP */
