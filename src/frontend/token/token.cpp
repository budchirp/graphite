#include <sstream>

#include "frontend/token/token.hpp"

Token::Token() {
  this->type = TokenType::TOKEN_EOF;
  this->literal = "";
}

Token::Token(const TokenType &token_type, const string &literal) {
  this->type = token_type;
  this->literal = literal;
};

string Token::to_string() const { return literal; }

string Token::to_string_tree() const {
  ostringstream oss;
  oss << "Token(type: " << static_cast<int>(type) << ", literal: '" << literal
      << "')";

  return oss.str();
}
