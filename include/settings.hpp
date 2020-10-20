#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings
{
  enum class Syntax
  {
    fahr,
    bnf,
    dummy,
  };

  Syntax syntax = Syntax::dummy;
};

#endif /* SETTINGS_H */
