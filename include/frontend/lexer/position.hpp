#pragma once

#include "std/types.hpp"

using namespace std;

class Position {
public:
  i32 line;
  i32 column;

  explicit Position(const i32 line, const i32 column);
};
