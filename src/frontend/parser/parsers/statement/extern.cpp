#include <memory>

#include "frontend/parser/parsers/statement/extern.hpp"
#include "frontend/parser/parsers/statement/proto.hpp"
#include "logger/log_types.hpp"

ExternStatementParser::ExternStatementParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<ExternStatement> ExternStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token(); // eat extern

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->logger->error("Expected identifier after extern",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto proto = ProtoStatementParser(parser).parse();
  return make_unique<ExternStatement>(position, std::move(proto));
}
