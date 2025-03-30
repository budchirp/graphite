#include "parser/parsers/statement/visibility.hpp"

#include "logger/log_types.hpp"
#include "parser/parsers/statement/extern.hpp"
#include "parser/parsers/statement/function.hpp"
#include "parser/parsers/statement/var.hpp"
#include "semantic/visibilty.hpp"

unique_ptr<Statement> VisibilityStatementParser::parse() {
  auto visibility_token = parser->current_token.type;

  // eat visibility indicator
  parser->eat_token();

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
}
