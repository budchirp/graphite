#include "parser/parsers/statement/block.hpp"

#include <memory>

#include "parser/parsers/statement/statement.hpp"
#include "types/type.hpp"

unique_ptr<BlockStatement> BlockStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat {

  auto statement_parser = StatementParser(parser);

  auto statements = vector<unique_ptr<Statement>>();
  shared_ptr<Type> last_statement_type;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_BRACE) {
    if (auto statement = statement_parser.parse()) {
      last_statement_type = statement->get_type();

      statements.push_back(std::move(statement));
    }
  }

  parser->eat_token();  // eat }

  return make_unique<BlockStatement>(position, last_statement_type, std::move(statements));
}
