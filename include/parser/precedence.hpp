#pragma once

#include <unordered_map>

#include "token/token_type.hpp"

enum class Precedence {
  LOWEST,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
  CALL
};

class PrecedenceHelper {
 public:
  static Precedence precedence_for(const TokenType &tokenType) {
    static const std::unordered_map<TokenType, Precedence> precedenceMap = {
        {TokenType::TOKEN_EQUAL, Precedence::EQUALS},
        {TokenType::TOKEN_NOT_EQUAL, Precedence::EQUALS},
        {TokenType::TOKEN_LESS_THAN, Precedence::LESSGREATER},
        {TokenType::TOKEN_GREATER_THAN, Precedence::LESSGREATER},
        {TokenType::TOKEN_PLUS, Precedence::SUM},
        {TokenType::TOKEN_MINUS, Precedence::SUM},
        {TokenType::TOKEN_SLASH, Precedence::PRODUCT},
        {TokenType::TOKEN_ASTERISK, Precedence::PRODUCT},
        {TokenType::TOKEN_LEFT_PARENTHESES, Precedence::CALL}};

    if (const auto it = precedenceMap.find(tokenType);
        it != precedenceMap.end()) {
      return it->second;
    }

    return Precedence::LOWEST;
  }
};
