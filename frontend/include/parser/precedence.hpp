#pragma once

#include <unordered_map>

#include "lexer/token/token_type.hpp"

class Precedence {
 public:
  enum Value {
    LOWEST,
    ASSIGN,       // Assignment operators like =
    EQUALS,       // Equality operators like ==, !=
    LESSGREATER,  // Comparison operators like <, >, <=, >=
    SUM,          // Additive operators like +, -
    PRODUCT,      // Multiplicative operators like *, /, %
    PREFIX,       // Unary prefix operators like !, -, &
    CALL,         // Function calls, array indexing, member access
    POSTFIX       // Unary postfix operators like ++, --
  };

  static Value precedence_for(const TokenType &tokenType) {
    static const std::unordered_map<TokenType, Value> precedence_map = {
        {TOKEN_ASSIGN, ASSIGN},
        {TOKEN_EQUAL, EQUALS},
        {TOKEN_NOT_EQUAL, EQUALS},
        {TOKEN_LESS_THAN, LESSGREATER},
        {TOKEN_GREATER_THAN, LESSGREATER},
        {TOKEN_PLUS, SUM},
        {TOKEN_MINUS, SUM},
        {TOKEN_SLASH, PRODUCT},
        {TOKEN_ASTERISK, PRODUCT},
        {TOKEN_AMPERSAND, PREFIX},       // &x is a prefix operator
        {TOKEN_BANG, PREFIX},            // !x is a prefix operator
        {TOKEN_PLUSPLUS, POSTFIX},       // x++ is a postfix operator
        {TOKEN_MINUSMINUS, POSTFIX},     // x-- is a postfix operator
        {TOKEN_LEFT_PARENTHESES, CALL},  // Function call: x()
        {TOKEN_LEFT_BRACKET, CALL},      // Array indexing: asd[1]
        {TOKEN_DOT, CALL},               // Member access: struct_type.asd
        {TOKEN_AS, CALL},                // Type casting: x as i32
        {TOKEN_BANG_BANG, CALL},         // Double bang (if it's an operator)
        {TOKEN_LEFT_BRACE, CALL}         // Struct literal/initialization
    };

    if (const auto it = precedence_map.find(tokenType);
        it != precedence_map.end()) {
      return it->second;
    }

    return LOWEST;
  }
};
