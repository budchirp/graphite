#include "lexer/position.hpp"

Position::Position(const int &line, const int &column) {
  this->line = line;
  this->column = column;
}
