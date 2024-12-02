#include <memory>

#include "frontend/parser/parser.hpp"

Parser::Parser(shared_ptr<Lexer> lexer) {
  this->lexer = lexer;
  if (!this->lexer) {
    throw runtime_error("Lexer is null!");
  }

  current_token = lexer->next_token();
  next_token = lexer->next_token();
}

void Parser::eat_token() {
  current_token = next_token;
  next_token = lexer->next_token();
}
