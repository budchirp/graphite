#pragma once

#include <memory>
#include <string>

#include "position.hpp"
#include "token/token.hpp"

using namespace std;

class Lexer {
 private:
  const string &source;

  int read_position;

  char next_char;
  char current_char;

  char get_current_char() const;

  void eat_char();
  void eat_whitespace();

 public:
  shared_ptr<Position> position;

  explicit Lexer(const string &source)
      : source(source),
        read_position(0),
        position(make_shared<Position>(1, 0)) {
    eat_char();
  };

  Token next_token();
};
