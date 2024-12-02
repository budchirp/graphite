#pragma once

#include <string>

#include "token_type.hpp"

using namespace std;

class Token {
public:
  TokenType type;
  string literal;

  Token();
  Token(TokenType token_type, string literal);

  string to_string() const;
  string to_string_tree() const;
};
