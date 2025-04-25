#include "parser/statement/statement.hpp"

#include <memory>

#include "lexer/token/token_type.hpp"
#include "parser/statement/expression.hpp"
#include "parser/statement/extern.hpp"
#include "parser/statement/for.hpp"
#include "parser/statement/function.hpp"
#include "parser/statement/include.hpp"
#include "parser/statement/return.hpp"
#include "parser/statement/struct.hpp"
#include "parser/statement/var.hpp"
#include "parser/statement/visibility.hpp"
#include "parser/statement/while.hpp"

shared_ptr<Statement> StatementParser::parse() {
  switch (parser->current_token.type) {
    case TOKEN_SEMICOLON:
    case TOKEN_EOF: {
      parser->eat_token();
      break;
    }

    case TOKEN_INCLUDE: {
      return IncludeStatementParser(parser).parse();
    }

    case TOKEN_PUBLIC:
    case TOKEN_PRIVATE: {
      return VisibilityStatementParser(parser).parse();
    }

    case TOKEN_STRUCT: {
      return StructStatementParser(parser).parse();
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
