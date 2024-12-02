#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expressions/binary.hpp"
#include "frontend/parser/parsers/expression/binary.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"

BinaryExpressionParser::BinaryExpressionParser(shared_ptr<Parser> parser,
                                               unique_ptr<Expression> left) {
  this->parser = parser;
  this->left = std::move(left);
}

unique_ptr<Expression> BinaryExpressionParser::parse() {
  auto operator_token = parser->current_token;
  parser->eat_token(); // eat operator

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto right = expression_statement_parser.parse_expression(
      PrecedenceHelper::precedence_for(operator_token.type));

  return make_unique<BinaryExpression>(operator_token, std::move(left),
                                       std::move(right));
}
