#include "parser/statement/visibility.hpp"

#include "logger/log_types.hpp"
#include "parser/statement/extern.hpp"
#include "parser/statement/function.hpp"
#include "parser/statement/var.hpp"
#include "semantic/visibilty.hpp"

shared_ptr<Statement> VisibilityStatementParser::parse() {
  auto visibility_token = parser->current_token.type;

  parser->eat_token();  // eat visibility indicator

  switch (parser->current_token.type) {
    case TOKEN_FUN: {
      return FunctionStatementParser(parser).parse(visibility_token);
    }

    case TOKEN_EXTERN: {
      return ExternStatementParser(parser).parse(visibility_token);
    }

    case TOKEN_VAR: {
      return VarStatementParser(parser).parse(
          SymbolVisibility::from_token_type(visibility_token));
    }

    default: {
      parser->get_logger()->error(
          "Unknown statement after visibility indicator!",
          LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  return nullptr;
}
