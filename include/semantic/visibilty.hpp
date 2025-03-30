#pragma once

#include "lexer/token/token_type.hpp"

class SymbolVisibility {
 public:
  enum Value { PUBLIC, PRIVATE, SCOPE };

  static Value from_token_type(const TokenType &token_type) {
    switch (token_type) {
      case TOKEN_PUBLIC:
        return Value::PUBLIC;
      case TOKEN_PRIVATE:
        return Value::PRIVATE;

      default:
        return Value::SCOPE;
    }
  }
};
