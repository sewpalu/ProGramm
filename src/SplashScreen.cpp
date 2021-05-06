#include "SplashScreen.hpp"

SplashScreen::SplashScreen(const std::filesystem::path& image_file,
                           const std::string& title)
    : m_splash_frame(new wxFrame{nullptr, wxID_ANY, title}),
      m_splash_screen(
          new wxSplashScreen{wxBitmap{image_file.c_str(), wxBITMAP_TYPE_PNG},
                             wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT, 0,
                             m_splash_frame, wxID_ANY})
{
  wxYield();
}

SplashScreen::~SplashScreen()
{
  m_splash_screen->Close();
  m_splash_frame->Close();
}
