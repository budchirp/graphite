#include "parser/parsers/statement/statement.hpp"

#include <memory>

#include "parser/parsers/statement/expression.hpp"
#include "parser/parsers/statement/extern.hpp"
#include "parser/parsers/statement/for.hpp"
#include "parser/parsers/statement/function.hpp"
#include "parser/parsers/statement/return.hpp"
#include "parser/parsers/statement/var.hpp"
#include "parser/parsers/statement/while.hpp"

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

    case TOKEN_FOR: {
      return ForStatementParser(parser).parse();
    }

    case TOKEN_WHILE: {
      return WhileStatementParser(parser).parse();
    }

    default:
      return ExpressionStatementParser(parser).parse();
  }

  return nullptr;
}
