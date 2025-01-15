#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/boolean.hpp"
#include "frontend/parser/parsers/expression/boolean.hpp"

BooleanExpressionParser::BooleanExpressionParser(
    const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> BooleanExpressionParser::parse() {
  const auto boolean_token = parser->current_token;
  parser->eat_token(); // eat boolean

  return make_unique<BooleanExpression>(
      *parser->get_lexer()->position,
      boolean_token.literal == "true" ? true : false);
}
