#pragma once

#include <vector>

#include "Terminal.hpp"

/**
 * Class representing a word (sequence of terminal symbols).
 *
 * This is a struct on the grounds of C++ Core Guideline C.2
 * See: http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-struct
 */
struct Word
{
  std::vector<Terminal> content;

  std::size_t getSize();

  Terminal at(int position);

  Word();
  explicit Word(std::vector<Terminal> content_input);
  ~Word();
};
