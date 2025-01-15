#include <memory>

#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/parsers/statement/return.hpp"
#include "frontend/parser/precedence.hpp"

ReturnStatementParser::ReturnStatementParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<ReturnStatement> ReturnStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token(); // eat return

  auto expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);
  return make_unique<ReturnStatement>(position, std::move(expression));
}
