#ifndef VISUALISATION_TAB_HPP
#define VISUALISATION_TAB_HPP

#include <functional>
#include <memory>
#include <optional>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "FormalGrammar.hpp"
#include "GUIVisualisationInterface.hpp"
#include "Visualisable.hpp"
#include "Word.hpp"

class StepsDisplay;
class VisualisationWidget;

/**
 * Superclass for tabs containing visualisations.
 */
class VisualisationTab : public wxPanel, public GUIVisualisationInterface
{
public:
  enum class DiagnosticsLevel : unsigned char
  {
    info,
    warn,
    error,
  };

protected:
  StepsDisplay* m_steps = nullptr;
  std::unique_ptr<Visualisable> m_visualised_thing;
  wxStaticText* m_diagnostics = nullptr;
  std::optional<FormalGrammar> m_current_grammar;
  std::optional<std::string> m_current_word;

public:
  virtual ~VisualisationTab() = default;

  /**
   * Updates the visualisation according to the provided input.
   */
  void update_input(const FormalGrammar& grammar, const std::string& word);

  ///
  /// Default implementations for GUIVisualisationInterface
  ///

  virtual void draw_tree(const SyntaxTree&) override{};
  // virtual void draw_tree(Tree* tree) override{};
  virtual void draw_table(const Table&) override{};
  virtual void draw_empty() override = 0;

  virtual void set_button(const std::string&, Callback,
                          Position = Position::left) override{};
  virtual void set_options(const std::vector<Option>& options) override{};

protected:
  virtual void render_input() = 0;

  void visualise();

  void update_visualisation();

  void show_diagnostics(const std::string& msg,
                        DiagnosticsLevel level = DiagnosticsLevel::info);
  void clear_diagnostics();

  /**
   * Indicates whether the parent notebook has this page selected
   */
  bool is_selected() const;
};

#endif /* ifndef VISUALISATION_TAB_HPP */
