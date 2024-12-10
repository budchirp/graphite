#include "frontend/lexer/position.hpp"

Position::Position(const i32 line, const i32 column) {
  this->line = line;
  this->column = column;
}
