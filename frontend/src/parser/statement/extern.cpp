#include "parser/statement/extern.hpp"

#include <memory>

#include "ast/statement/extern.hpp"
#include "logger/log_types.hpp"
#include "parser/statement/proto.hpp"
#include "semantic/visibilty.hpp"

shared_ptr<ExternStatement> ExternStatementParser::parse() {
  return parse(TOKEN_PRIVATE);
}

shared_ptr<ExternStatement> ExternStatementParser::parse(
    const TokenType &visibility) const {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat extern

  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after extern",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto proto_statement = ProtoStatementParser(parser).parse();
  if (!proto_statement) {
    parser->get_logger()->error("Failed to parse statement",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<ExternStatement>(
      position, SymbolVisibility::from_token_type(visibility),
      proto_statement);
}
