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
        {TOKEN_AS, Precedence::CALL},
        {TOKEN_EQUAL, Precedence::EQUALS},
        {TOKEN_NOT_EQUAL, Precedence::EQUALS},
        {TOKEN_LESS_THAN, Precedence::LESSGREATER},
        {TOKEN_GREATER_THAN, Precedence::LESSGREATER},
        {TOKEN_PLUS, Precedence::SUM},
        {TOKEN_MINUS, Precedence::SUM},
        {TOKEN_PLUSPLUS, Precedence::SUM},
        {TOKEN_MINUSMINUS, Precedence::SUM},
        {TOKEN_SLASH, Precedence::PRODUCT},
        {TOKEN_ASTERISK, Precedence::PRODUCT},
        {TOKEN_LEFT_PARENTHESES, Precedence::CALL}};

    if (const auto it = precedenceMap.find(tokenType);
        it != precedenceMap.end()) {
      return it->second;
    }

    return Precedence::LOWEST;
  }
};
