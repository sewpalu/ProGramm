#include "ManagerTab.hpp"

#include "ProductionManager.hpp"
#include "wx/html/forcelnk.h"
#include "wx/xrc/xmlres.h"

#include <iostream>
#include <optional>
#include <stdexcept>

FORCE_LINK_ME(ManagerTab);

wxIMPLEMENT_DYNAMIC_CLASS(ManagerTab, wxPanel);

BEGIN_EVENT_TABLE(ManagerTab, wxPanel)

EVT_WINDOW_CREATE(ManagerTab::on_create)
EVT_CHILD_FOCUS(ManagerTab::on_page_changed)
EVT_ERASE_BACKGROUND(ManagerTab::on_erase_background)
END_EVENT_TABLE()

ManagerTab::ManagerTab()
{
  Show();
}

void ManagerTab::on_page_changed(wxChildFocusEvent&)
{
  Layout();
}

void ManagerTab::on_create(wxWindowCreateEvent& evt)
{
  if (evt.GetWindow() != dynamic_cast<wxWindow*>(this))
    return;

  auto* sizer = new wxBoxSizer{wxVERTICAL};
  auto* panel = wxXmlResource::Get()->LoadPanel(this, "manager_panel");
  sizer->Add(panel, wxSizerFlags{}.Expand().Border(wxALL, 5).Proportion(1));
  SetSizer(sizer);

  this->old_size = this->GetSize();

  this->grammar_steps =
      dynamic_cast<wxNotebook*>(FindWindowByName("manager_steps"));
  if (!this->grammar_steps)
  {
    std::cerr << "Unable to load grammar manager steps notebook.\n";
    return;
  }
  this->grammar_steps->Bind<>(wxEVT_NOTEBOOK_PAGE_CHANGED,
                              &ManagerTab::page_changed, this);

  this->alpha_manager =
      dynamic_cast<AlphabetManager*>(FindWindowByName("manager_alphabet_tab"));
  if (!this->alpha_manager)
  {
    std::cerr << "Unable to load alphabet tab in manager.\n";
    return;
  }

  this->prod_manager = dynamic_cast<ProductionManager*>(
      FindWindowByName("manager_production_tab"));
  if (!this->prod_manager)
  {
    std::cerr << "Unable to load productions tab in manager\n";
    return;
  }

  this->overview_tab = dynamic_cast<GrammarOverviewTab*>(
      FindWindowByName("manager_overview_tab"));
  if (!this->overview_tab)
  {
    std::cerr << "Unable to load overview tab in manager\n";
    return;
  }

  auto* activation_button =
      dynamic_cast<wxButton*>(FindWindowByName("manager_activate_button"));
  if (!activation_button)
  {
    std::cerr << "Unable to load activation button in manager\n";
    return;
  }
  activation_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                          &ManagerTab::activate_grammar, this);

  this->app_grammar_panel =
      dynamic_cast<GrammarEditor*>(FindWindowByName("grammar_editor_side"));
  if (!this->app_grammar_panel)
  {
    std::cerr << "Unable to load grammar side panel in manager\n";
    return;
  }

  Layout();
}

void ManagerTab::load_grammar_from_tab(const wxWindow* tab)
{
  if (tab == this->alpha_manager)
  {
    this->terminal_alphabet = this->alpha_manager->get_terminal_alphabet();
    this->nonterminal_alphabet =
        this->alpha_manager->get_nonterminal_alphabet();
    this->start_symbol = this->alpha_manager->get_start_symbol();
  }
  else if (tab == this->prod_manager)
  {
    this->productions = this->prod_manager->get_productions();
  }
  else if (tab == this->overview_tab)
  {
    this->nonterminal_alphabet = this->overview_tab->get_nonterminal_alphabet();
    this->terminal_alphabet = this->overview_tab->get_terminal_alphabet();
    this->productions = this->overview_tab->get_productions();
    try
    {
      this->start_symbol = FormalGrammar::choose_startsymbol(
                               this->nonterminal_alphabet, this->productions)
                               .start;
    }
    catch (std::runtime_error)
    {
      this->start_symbol = {};
    }
  }
}

void ManagerTab::page_changed(wxBookCtrlEvent& evt)
{
  // Check that there is a valid page changed from
  if (!(evt.GetOldSelection() < 0))
  {
    load_grammar_from_tab(this->grammar_steps->GetPage(evt.GetOldSelection()));
  }

  if (this->grammar_steps->GetCurrentPage() == this->alpha_manager)
  {
    this->alpha_manager->set_nonterminal_alphabet(this->nonterminal_alphabet);
    this->alpha_manager->set_terminal_alphabet(this->terminal_alphabet);
    this->alpha_manager->Refresh();
  }
  else if (this->grammar_steps->GetCurrentPage() == this->prod_manager)
  {
    this->prod_manager->set_nonterminal_alphabet(this->nonterminal_alphabet);
    this->prod_manager->set_terminal_alphabet(this->terminal_alphabet);
    this->prod_manager->set_productions(this->productions);
    this->prod_manager->Refresh();
  }
  else if (this->grammar_steps->GetCurrentPage() == this->overview_tab)
  {
    this->overview_tab->set_nonterminal_alphabet(this->nonterminal_alphabet);
    this->overview_tab->set_terminal_alphabet(this->terminal_alphabet);
    this->overview_tab->set_productions(this->productions);
    this->overview_tab->Refresh();
  }
}

void ManagerTab::on_erase_background(wxEraseEvent& event)
{
  // Erasing the background would cause flickering of the GUI elements.
  // However in this case erasing is not necessary,
  // so the event is caught and not handled.
}

void ManagerTab::activate_grammar(wxCommandEvent&)
{
  load_grammar_from_tab(this->grammar_steps->GetCurrentPage());
  app_grammar_panel->set_grammar(
      {.rules = this->productions,
       .start = this->start_symbol.value_or(Nonterminal{""})});
}
