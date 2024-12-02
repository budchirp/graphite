#include <memory>

#include "frontend/parser/parsers/statement/block.hpp"
#include "frontend/parser/parsers/statement/function.hpp"
#include "frontend/parser/parsers/statement/proto.hpp"
#include "utils/logger/logger.hpp"

FunctionStatementParser::FunctionStatementParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<FunctionStatement> FunctionStatementParser::parse() {
  parser->eat_token(); // eat fn

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    Logger::error("Expected identifier after fn");
    return nullptr;
  }

  auto proto_statement_parser = ProtoStatementParser(parser);
  auto proto = proto_statement_parser.parse();

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    Logger::error("Expected left brace after proto");
    return nullptr;
  }

  auto block_statement_parser = BlockStatementParser(parser);
  auto body = block_statement_parser.parse();

  return make_unique<FunctionStatement>(std::move(proto), std::move(body));
}
