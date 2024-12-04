#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expressions/integer.hpp"
#include "frontend/parser/parsers/expression/integer.hpp"

IntegerExpressionParser::IntegerExpressionParser(shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<Expression> IntegerExpressionParser::parse() {
  auto integer_token = parser->current_token;
  parser->eat_token(); // eat integer

  return make_unique<IntegerExpression>(stoi(integer_token.literal));
}
