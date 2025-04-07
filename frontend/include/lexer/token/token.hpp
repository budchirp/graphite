#pragma once

#include <string>

#include "lexer/token/token_type.hpp"

using namespace std;

class Token {
 public:
  TokenType type;
  string literal;

  explicit Token();
  explicit Token(const TokenType &token_type, const string &literal);

  string to_string() const;
  string to_string_tree() const;
};
