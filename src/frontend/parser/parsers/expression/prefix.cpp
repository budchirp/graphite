#include <memory>

#include "frontend/ast/expressions/prefix.hpp"
#include "frontend/parser/parsers/expression/prefix.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"

PrefixExpressionParser::PrefixExpressionParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<Expression> PrefixExpressionParser::parse() {
  auto prefix_token = parser->current_token;
  parser->eat_token(); // eat prefix

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto right = expression_statement_parser.parse_expression(
      PrecedenceHelper::precedence_for(prefix_token.type));

  return make_unique<PrefixExpression>(prefix_token, std::move(right));
}
