#include <memory>

#include "frontend/parser/parsers/statement/block.hpp"
#include "frontend/parser/parsers/statement/function.hpp"
#include "frontend/parser/parsers/statement/proto.hpp"
#include "logger/log_types.hpp"

FunctionStatementParser::FunctionStatementParser(
    const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<FunctionStatement> FunctionStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token(); // eat fn

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->logger->error("Expected identifier after fn",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto proto = ProtoStatementParser(parser).parse();

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->logger->error("Expected left brace after function prototype",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto body = BlockStatementParser(parser).parse();
  return make_unique<FunctionStatement>(position, std::move(proto), std::move(body));
}
