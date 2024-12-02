#include <memory>

#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/parsers/statement/return.hpp"
#include "frontend/parser/precedence.hpp"

ReturnStatementParser::ReturnStatementParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<ReturnStatement> ReturnStatementParser::parse() {
  parser->eat_token(); // eat return

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto expression =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  return make_unique<ReturnStatement>(std::move(expression));
}
