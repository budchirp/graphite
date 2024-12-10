#include <memory>

#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/parsers/statement/extern.hpp"
#include "frontend/parser/parsers/statement/function.hpp"
#include "frontend/parser/parsers/statement/return.hpp"
#include "frontend/parser/parsers/statement/statement.hpp"
#include "frontend/parser/parsers/statement/var.hpp"
#include "frontend/token/token_type.hpp"

StatementParser::StatementParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Statement> StatementParser::parse() {
  switch (parser->current_token.type) {
  case TOKEN_SEMICOLON:
  case TOKEN_EOF: {
    parser->eat_token();
    break;
  }

  case TOKEN_FUN: {
    return FunctionStatementParser(parser).parse();
  }

  case TOKEN_EXTERN: {
    return ExternStatementParser(parser).parse();
  }

  case TOKEN_VAR: {
    return VarStatementParser(parser).parse();
  }

  case TOKEN_RETURN: {
    return ReturnStatementParser(parser).parse();
  }

  default:
    return ExpressionStatementParser(parser).parse();
  }

  return nullptr;
}
