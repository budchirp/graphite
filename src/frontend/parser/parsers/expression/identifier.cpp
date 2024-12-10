#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/identifier.hpp"
#include "frontend/parser/parsers/expression/identifier.hpp"

IdentifierExpressionParser::IdentifierExpressionParser(
    const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> IdentifierExpressionParser::parse() {
  auto identifier_token = parser->current_token;
  parser->eat_token(); // eat identifier

  return make_unique<IdentifierExpression>(identifier_token.literal);
}
