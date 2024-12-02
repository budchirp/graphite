#include <memory>

#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/parsers/statement/extern.hpp"
#include "frontend/parser/parsers/statement/function.hpp"
#include "frontend/parser/parsers/statement/return.hpp"
#include "frontend/parser/parsers/statement/statement.hpp"
#include "frontend/parser/parsers/statement/var.hpp"
#include "frontend/token/token_type.hpp"

StatementParser::StatementParser(shared_ptr<Parser> parser) {
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
    auto function_statement_parser = FunctionStatementParser(parser);
    return function_statement_parser.parse();
  }

  case TOKEN_EXTERN: {
    auto extern_statement_parser = ExternStatementParser(parser);
    return extern_statement_parser.parse();
  }

  case TOKEN_VAR: {
    auto var_statement_parser = VarStatementParser(parser);
    return var_statement_parser.parse();
  }

  case TOKEN_RETURN: {
    auto return_statement_parser = ReturnStatementParser(parser);
    return return_statement_parser.parse();
  }

  default:
    auto expression_statement_parser = ExpressionStatementParser(parser);
    return expression_statement_parser.parse();
  }

  return nullptr;
}
