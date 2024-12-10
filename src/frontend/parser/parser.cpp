#include <memory>

#include "frontend/parser/parser.hpp"

Parser::Parser(shared_ptr<Lexer> lexer) {
  this->lexer = lexer;

  logger = make_shared<Logger>(lexer->position);

  current_token = lexer->next_token();
  next_token = lexer->next_token();
}

void Parser::eat_token() {
  current_token = next_token;
  next_token = lexer->next_token();
}
