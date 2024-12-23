#pragma once

#include <memory>
#include <string>

#include "frontend/token/token.hpp"
#include "position.hpp"
#include "std/types.hpp"

using namespace std;

class Lexer {
private:
  const string &source;

  i32 read_position;

  i8 next_char;
  i8 current_char;

  i8 get_current_char() const;

  void eat_char();
  void eat_whitespace();

  Token tokenize(i8 character);

public:
  shared_ptr<Position> position;

  explicit Lexer(const string &source);

  Token next_token();
};
