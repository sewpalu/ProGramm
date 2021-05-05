#ifndef SPLASH_SCREEN_HPP
#define SPLASH_SCREEN_HPP

#include <filesystem>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/splash.h"

/**
 * RAII wrapper for displaying and closing a wxWidgets splash screen
 */
class SplashScreen
{
  private:
    wxFrame* m_splash_frame;
    wxSplashScreen* m_splash_screen;

public:
  /**
   * \param image_file Path to the image; Assumed to be a png
   * \param title      Window title for the containing frame
   */
  SplashScreen(const std::filesystem::path& image_file, const std::string& title);
  ~SplashScreen();
};

#endif /* ifndef SPLASH_SCREEN_HPP */
