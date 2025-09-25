#include "parser/statement/extern.hpp"

#include <memory>

#include "ast/statement/extern.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/statement/proto.hpp"
#include "parser/statement/var.hpp"
#include "semantic/visibilty.hpp"

shared_ptr<ExternStatement> ExternStatementParser::parse() {
  return parse(TOKEN_PRIVATE);
}

shared_ptr<ExternStatement> ExternStatementParser::parse(
    const TokenType &visibility) const {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat extern

  if (parser->current_token.type != TOKEN_FUN &&
      parser->current_token.type != TOKEN_VAR) {
    parser->get_logger()->error("Expected fn or var after extern",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  const auto token_type = parser->current_token.type;

  parser->eat_token();  // eat fn or var

  shared_ptr<Statement> proto_statement;
  if (token_type == TOKEN_FUN) {
    auto function_proto_statement =
        FunctionProtoStatementParser(parser).parse();
    if (!function_proto_statement) {
      parser->get_logger()->error("Failed to parse function proto statement",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    proto_statement = function_proto_statement;
  } else {
    auto var_proto_statement = VarProtoStatementParser(parser).parse();
    if (!var_proto_statement) {
      parser->get_logger()->error("Failed to parse var proto statement",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    proto_statement = var_proto_statement;
  }

  return make_shared<ExternStatement>(
      position, SymbolVisibility::from_token_type(visibility), proto_statement);
}
