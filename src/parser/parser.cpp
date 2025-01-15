#include <memory>

#include "parser/parser.hpp"

Parser::Parser(shared_ptr<Lexer> lexer, shared_ptr<Env> env)
    : lexer(lexer), env(env) {
  logger = make_shared<Logger>(lexer->position);

  current_token = lexer->next_token();
  next_token = lexer->next_token();
}

void Parser::eat_token() {
  current_token = next_token;
  next_token = lexer->next_token();
}
