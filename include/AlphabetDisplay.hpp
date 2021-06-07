#ifndef ALPHABET_DISPLAY_HPP
#define ALPHABET_DISPLAY_HPP

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/wrapsizer.h>
#include <wx/panel.h>
#include <wx/listbox.h>

#include <vector>
#include <Terminal.hpp>
#include <Nonterminal.hpp>

class AlphabetDisplay : public wxPanel
{
public:
  AlphabetDisplay();

  void set_alphabet(std::vector<Nonterminal*> nonterminals,
                    std::vector<Terminal*> terminals);

private:
  /**
   * Used to load XRC resources.
   * This is necessary because this wxPanel isn't complete at construction time.
   */
  void on_create(wxWindowCreateEvent& evt);

  std::vector<Terminal*> m_terminals;
  std::vector<Nonterminal*> m_nonterminals;

  wxListBox* m_terminal_display;
  wxListBox* m_nonterminal_display;

  wxDECLARE_DYNAMIC_CLASS(AlphabetDisplay);
  wxDECLARE_EVENT_TABLE();
};

#endif /* ifndef ALPHABET_DISPLAY_HPP */
