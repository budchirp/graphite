#pragma once

class Position {
 public:
  int line;
  int column;

  explicit Position(const int &line, const int &column)
      : line(line), column(column) {};
};
