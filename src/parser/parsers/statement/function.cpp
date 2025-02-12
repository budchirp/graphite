#include "parser/parsers/statement/function.hpp"

#include <memory>

#include "logger/log_types.hpp"
#include "parser/parsers/statement/block.hpp"
#include "parser/parsers/statement/proto.hpp"

unique_ptr<FunctionStatement> FunctionStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat fn

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after fn",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto proto_statement = ProtoStatementParser(parser).parse();
  if (!proto_statement) {
    parser->get_logger()->error("Failed to parse the proto of the function",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after function prototype",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto name = proto_statement->name->get_identifier();
  auto scope = make_shared<Scope>(name,
      parser->get_program()->get_context()->get_env()->get_current_scope());
  parser->get_program()->get_context()->get_env()->add_scope(
      name, scope);

  auto body_statement = BlockStatementParser(parser).parse();
  if (!body_statement) {
    parser->get_logger()->error("Failed to parse the body of the function",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<FunctionStatement>(
      position, scope, std::move(proto_statement), std::move(body_statement));
}
