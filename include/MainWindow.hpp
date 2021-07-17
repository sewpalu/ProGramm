#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP value

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

/**
 * Main window of the application.
 *
 * Automagically loads the UI from the XRC resource file.
 */
class MainWindow : public wxFrame
{
public:
  MainWindow();
};

#endif /* ifndef MAIN_WINDOW_HPP */
