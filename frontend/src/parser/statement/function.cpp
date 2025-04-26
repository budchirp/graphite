#include "parser/statement/function.hpp"

#include <memory>

#include "ast/statement/function.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/statement/block.hpp"
#include "parser/statement/proto.hpp"

shared_ptr<FunctionStatement> FunctionStatementParser::parse() {
  return parse(TOKEN_PRIVATE);
}

shared_ptr<FunctionStatement> FunctionStatementParser::parse(
    const TokenType &visibility) {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat fn

  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after fn",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto proto_statement = ProtoStatementParser(parser).parse();
  if (!proto_statement) {
    parser->get_logger()->error("Failed to parse statement",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after function prototype",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto name = proto_statement->name->value;
  auto scope = make_shared<Scope>(
      name,
      parser->get_program()->get_context()->get_env()->get_current_scope());
  parser->get_program()->get_context()->get_env()->add_scope(name, scope);

  auto body_statement = BlockStatementParser(parser).parse();
  if (!body_statement) {
    parser->get_logger()->error("Failed to parse statement",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<FunctionStatement>(
      position, scope, SymbolVisibility::from_token_type(visibility),
      proto_statement, body_statement);
}
