#include <memory>

#include "frontend/parser/parsers/statement/block.hpp"
#include "frontend/parser/parsers/statement/statement.hpp"

BlockStatementParser::BlockStatementParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<BlockStatement> BlockStatementParser::parse() {
  parser->eat_token(); // eat {

  auto statement_parser = StatementParser(parser);

  auto statements = std::vector<std::unique_ptr<Statement>>();
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_BRACE) {
    auto statement = statement_parser.parse();
    if (statement) {
      statements.push_back(std::move(statement));
    }
  }

  parser->eat_token(); // eat }

  return make_unique<BlockStatement>(std::move(statements));
}
