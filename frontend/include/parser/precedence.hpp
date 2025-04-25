#pragma once

#include <unordered_map>

#include "lexer/token/token_type.hpp"

class Precedence {
 public:
  enum Value { LOWEST, EQUALS, LESSGREATER, SUM, PRODUCT, PREFIX, CALL };

  static Value precedence_for(const TokenType &tokenType) {
    static const std::unordered_map<TokenType, Value> precedence_map = {
        {TOKEN_LEFT_BRACKET, CALL},
        {TOKEN_LEFT_BRACE, CALL},
        {TOKEN_AS, CALL},
        {TOKEN_ASSIGN, CALL},
        {TOKEN_BANG_BANG, CALL},
        {TOKEN_EQUAL, EQUALS},
        {TOKEN_NOT_EQUAL, EQUALS},
        {TOKEN_LESS_THAN, LESSGREATER},
        {TOKEN_GREATER_THAN, LESSGREATER},
        {TOKEN_PLUS, SUM},
        {TOKEN_MINUS, SUM},
        {TOKEN_PLUSPLUS, SUM},
        {TOKEN_MINUSMINUS, SUM},
        {TOKEN_SLASH, PRODUCT},
        {TOKEN_ASTERISK, PRODUCT},
        {TOKEN_LEFT_PARENTHESES, CALL}};

    if (const auto it = precedence_map.find(tokenType);
        it != precedence_map.end()) {
      return it->second;
    }

    return LOWEST;
  }
};
