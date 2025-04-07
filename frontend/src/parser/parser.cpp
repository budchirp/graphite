#include "parser/parser.hpp"

#include <memory>

#include "program/program.hpp"

Parser::Parser(const shared_ptr<Lexer> &lexer,
               const shared_ptr<Program> &program)
    : lexer(lexer), program(program) {
  logger = make_shared<Logger>(lexer->position);

  current_token = lexer->next_token();
  next_token = lexer->next_token();
}

void Parser::eat_token() {
  current_token = next_token;
  next_token = lexer->next_token();
}
