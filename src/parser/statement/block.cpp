#include "parser/statement/block.hpp"

#include <memory>

#include "parser/statement/statement.hpp"

unique_ptr<BlockStatement> BlockStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat {

  auto statement_parser = StatementParser(parser);

  auto statements = vector<unique_ptr<Statement>>();
  while (parser->current_token.type != TOKEN_RIGHT_BRACE) {
    if (auto statement = statement_parser.parse()) {
      statements.push_back(std::move(statement));
    }
  }

  parser->eat_token();  // eat }

  return make_unique<BlockStatement>(position, std::move(statements));
}
