#include <memory>

#include "frontend/ast/expression/prefix.hpp"
#include "frontend/parser/parsers/expression/prefix.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"

PrefixExpressionParser::PrefixExpressionParser(
    const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> PrefixExpressionParser::parse() {
  const auto prefix_token = parser->current_token;
  parser->eat_token(); // eat prefix

  auto right = ExpressionStatementParser(parser).parse_expression(
      PrecedenceHelper::precedence_for(prefix_token.type));

  return make_unique<PrefixExpression>(*right->get_position(), prefix_token,
                                       std::move(right));
}
