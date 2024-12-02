#include <memory>

#include "frontend/parser/parsers/statement/extern.hpp"
#include "frontend/parser/parsers/statement/proto.hpp"
#include "utils/logger/logger.hpp"

ExternStatementParser::ExternStatementParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<ExternStatement> ExternStatementParser::parse() {
  parser->eat_token(); // eat extern

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    Logger::error("Expected identifier after extern");
    return nullptr;
  }

  auto proto_statement_parser = ProtoStatementParser(parser);
  auto proto = proto_statement_parser.parse();

  return make_unique<ExternStatement>(std::move(proto));
}
