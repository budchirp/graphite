#include <memory>

#include "frontend/ast/program.hpp"
#include "frontend/parser/parsers/statement/program.hpp"
#include "frontend/parser/parsers/statement/statement.hpp"

ProgramParser::ProgramParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

shared_ptr<Program> ProgramParser::parse() {
  auto statement_parser = StatementParser(parser);

  auto statements = std::vector<std::unique_ptr<Statement>>();
  while (parser->current_token.type != TokenType::TOKEN_EOF) {
    if (auto statement = statement_parser.parse()) {
      statements.push_back(std::move(statement));
    }
  }

  return std::make_shared<Program>(std::move(statements));
}
