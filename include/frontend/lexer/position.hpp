#pragma once

#include "std/types.hpp"

using namespace std;

class Position {
public:
  i32 line;
  i32 column;

  Position(i32 line, i32 column);
};
