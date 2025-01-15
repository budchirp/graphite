#include <memory>

#include "logger/log_types.hpp"
#include "parser/parsers/statement/block.hpp"
#include "parser/parsers/statement/function.hpp"
#include "parser/parsers/statement/proto.hpp"

unique_ptr<FunctionStatement> FunctionStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token(); // eat fn

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after fn",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto env = make_shared<Env>(parser->get_env());
  parser->set_env(env);

  auto proto = ProtoStatementParser(parser).parse();
  if (!proto) {
    parser->get_logger()->error("Failed to parse the proto of the function");
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after function prototype",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto body = BlockStatementParser(parser).parse();
  if (!body) {
    parser->get_logger()->error("Failed to parse the body of the function");
    return nullptr;
  }

  parser->set_env(env->get_parent());

  return make_unique<FunctionStatement>(position, env, std::move(proto),
                                        std::move(body));
}
