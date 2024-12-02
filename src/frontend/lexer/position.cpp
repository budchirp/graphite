#include "frontend/lexer/position.hpp"

Position::Position(i32 line, i32 column) {
  this->line = line;
  this->column = column;
}
