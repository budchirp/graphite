#include "program/parser.hpp"

#include <memory>

#include "parser/statement/statement.hpp"

void ProgramParser::parse() const {
  auto statement_parser = StatementParser(parser);

  auto statements = vector<shared_ptr<Statement>>();
  while (parser->current_token.type != TokenType::TOKEN_EOF) {
    if (auto statement = statement_parser.parse()) {
      parser->get_program()->statements.push_back(statement);
    }
  }
}
