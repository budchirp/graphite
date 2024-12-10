#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/boolean.hpp"
#include "frontend/parser/parsers/expression/boolean.hpp"

BooleanExpressionParser::BooleanExpressionParser(
    const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> BooleanExpressionParser::parse() {
  auto boolean_token = parser->current_token;
  parser->eat_token(); // eat boolean

  return make_unique<BooleanExpression>(
      boolean_token.literal == "true" ? true : false);
}
